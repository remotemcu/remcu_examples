/**
  ******************************************************************************
  * @file    SMBUS/SMBUS_TSENSOR/Src/main.c 
  * @author  MCD Application Team
  * @brief   This sample code shows how to use STM32L0xx SMBUS HAL API.
  *          The communication is done using an Temperature Sensor STLM75
  *          on STM32L073Z-EVAL Eval board.
  *          =================================================================== 
  *          Notes:
  *           - This example is intended for STM32L0xx families devices only. 
  *          =================================================================== 
  *          Notes:
  *           - The Temperature Sensor (STLM75) is compatible
  *             with the SMBUS protocol.
  *          ===================================================================
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stlogo.h"

/** @addtogroup STM32L0xx_HAL_Examples
  * @{
  */

/** @addtogroup SMBUS_TSENSOR
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define TSENSOR_ADDR            0x92    /* STLM75 Address  */
#define TEMPERATURE_LOW         26      /* 24°C             */
#define TEMPERATURE_HIGH        28      /* 26°C             */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
TSENSOR_InitTypeDef TSENSOR_InitStructure;

/* Useful variables */
uint16_t temperaturevalue = 0;
uint8_t  alertoccurs = 0;
uint8_t  requestsample = 0;
uint32_t tick = 0;
uint8_t  addressalert = 0;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void Display_ExampleDescription(void);
static void TSENSOR_SetHint(void);
static void TSENSOR_Display_Temperature(uint16_t Temperature);
static void Error_Handler(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /* STM32L0xx HAL library initialization:
       - Configure the Flash prefetch
       - Systick timer is configured by default as source of time base, but user 
         can eventually implement his proper time base source (a general purpose 
         timer for example or other time source), keeping in mind that Time base 
         duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and 
         handled in milliseconds basis.
       - Low Level Initialization
     */
  HAL_Init();
  
  /* Configure the system clock to 32 MHz */
  SystemClock_Config();
  
  /* Configure LED_RED */
  BSP_LED_Init(LED_RED);

  /* Configure Wkup/Tamper push-button */
  BSP_PB_Init(BUTTON_TAMPER, BUTTON_MODE_GPIO);

  /*##-1- Initialize the LCD #################################################*/
  BSP_LCD_Init();
   
  /*##-2- Display welcome messages on LCD ####################################*/  
  Display_ExampleDescription();

  /* Wait for Wkup/Tamper push-button press before starting the Example */
  while (BSP_PB_GetState(BUTTON_TAMPER) != GPIO_PIN_SET)
  {
  }
  
  /* Wait for Wkup/Tamper push-button release before starting the Example */
  while (BSP_PB_GetState(BUTTON_TAMPER) != GPIO_PIN_RESET)
  {
  }

  /*##-3- Display Example Template ###########################################*/  
  TSENSOR_SetHint();
  
  /*Display advice to heat thermometer*/
  BSP_LCD_SetFont(&Font16);
  
  /*##-4- Configure the Temperature Sensor ###################################*/
  /* Conversion 12 bits in continuous mode at one conversion per second */
  /* Alert outside range Limit Temperature */
  TSENSOR_InitStructure.AlertMode             = TSENSOR_INTERRUPT_MODE;
  TSENSOR_InitStructure.ConversionMode        = TSENSOR_CONTINUOUS_MODE;
  TSENSOR_InitStructure.TemperatureLimitHigh  = TEMPERATURE_HIGH;
  TSENSOR_InitStructure.TemperatureLimitLow   = TEMPERATURE_LOW;

  if (TSENSOR_Init(TSENSOR_ADDR, &TSENSOR_InitStructure) != TSENSOR_OK)
  {
    /* Initialization Error */
    BSP_LCD_SetTextColor(LCD_COLOR_RED);    
    BSP_LCD_DisplayStringAt(0, 115, (uint8_t*)"Initialization problem", CENTER_MODE); 
    Error_Handler();
  }

  /* Initialize tick counter */
  tick = HAL_GetTick();
  
  /*##-5- Main loop to manage Alert and display Temperature Measured #########*/
  while (1)
  {
    /* Check if a new temperature read is requested to display                */
    if (requestsample == 1)
    {
      /* Read and Display the current temperature                             */
      temperaturevalue = TSENSOR_ReadTemp(TSENSOR_ADDR);

      /* If integer temperature is inside threshold LOW and HIGH temperature, clear warning message */
      if (((temperaturevalue >> 4) > TEMPERATURE_LOW) && ((temperaturevalue >> 4) < TEMPERATURE_HIGH))
      {
        /* Clear previous warning message */
        BSP_LCD_SetTextColor(LCD_COLOR_WHITE);    
        BSP_LCD_DisplayStringAt(0, 160, (uint8_t *)"                       ", CENTER_MODE);
        BSP_LCD_DisplayStringAt(0, 175, (uint8_t *)"                       ", CENTER_MODE);
      }

      TSENSOR_Display_Temperature(temperaturevalue);
      
      /* Reset the sampling request                                           */
      tick = HAL_GetTick();
      requestsample = 0;
    }
    else
    {
      /* Request a Temperature sampling each 1s <-> 1000 ms                   */
      if (HAL_GetTick() >= tick + 1000)
      {
        /* Set the sampling request                                           */
        requestsample = 1;
      }
    }
    
    /* Check if an alert occurs                                               */
    if (alertoccurs == 1)
    {
      /* Display warning message depends on Limit value                       */
      if ((TSENSOR_ReadTemp(TSENSOR_ADDR) >> 4) >= TEMPERATURE_HIGH)
      {
        BSP_LCD_SetTextColor(LCD_COLOR_RED);
        /* Display warning message Temperature high limit exceeded            */
        BSP_LCD_DisplayStringAt(0, 160, (uint8_t *)"Temperature Limit High", CENTER_MODE);
        BSP_LCD_DisplayStringAt(0, 175, (uint8_t *)" has been exceeded ", CENTER_MODE);
      }
      else
      {
        BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
        /* Display warning message Temperature is at or blow low limit        */
        BSP_LCD_DisplayStringAt(0, 160, (uint8_t *)" Temperature is at or ", CENTER_MODE);
        BSP_LCD_DisplayStringAt(0, 175, (uint8_t *)"below the Low Limit", CENTER_MODE);
      }
      alertoccurs = 0;
    }
        
    HAL_Delay(5);
  }
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 32000000
  *            HCLK(Hz)                       = 32000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            APB2 Prescaler                 = 1
  *            Flash Latency(WS)              = 1
  *            Main regulator output voltage  = Scale1 mode
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct ={0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  
  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();
  
  /* The voltage scaling allows optimizing the power consumption when the device is 
     clocked below the maximum system frequency, to update the voltage scaling value 
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  
  /* Enable HSE Oscillator */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLSource   = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLState    = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLMUL      = RCC_PLL_MUL8;
  RCC_OscInitStruct.PLL.PLLDIV      = RCC_PLL_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct)!= HAL_OK)
  {
    /* Initialization Error */
    while(1); 
  }
  
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;  
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1)!= HAL_OK)
  {
    /* Initialization Error */
    while(1); 
  }
}


/**
  * @brief  Display main example message
  * @param  None
  * @retval None
  */
static void Display_ExampleDescription(void)
{
  BSP_LCD_SetFont(&LCD_DEFAULT_FONT);
  
  /* Clear the LCD */ 
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE); 
  BSP_LCD_Clear(LCD_COLOR_WHITE);

  /* Set the LCD Text Color */
  BSP_LCD_SetTextColor(LCD_COLOR_DARKBLUE);  

  /* Display LCD messages */
  BSP_LCD_DisplayStringAt(0, 10, (uint8_t *)"STM32L0xx", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 35, (uint8_t *)"Example", CENTER_MODE);
  /* Draw Bitmap */
  BSP_LCD_DrawBitmap((BSP_LCD_GetXSize() - 80)/2, 65, (uint8_t *)stlogo);
  
  BSP_LCD_SetFont(&Font12);
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()- 20, (uint8_t *)"Copyright (c) STMicroelectronics 2016", CENTER_MODE);
  
  BSP_LCD_SetFont(&Font16);
  BSP_LCD_SetTextColor(LCD_COLOR_DARKBLUE);
  BSP_LCD_FillRect(0, BSP_LCD_GetYSize()/2 + 15, BSP_LCD_GetXSize(), 60);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_DARKBLUE); 
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()/2 + 15, (uint8_t *)"Press Wkup/Tamper push-button", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()/2 + 30, (uint8_t *)"to start :", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()/2 + 45, (uint8_t *)" TEMPERATURE SENSOR Example", CENTER_MODE);
}

/**
  * @brief  Display TSENSOR Demo Hint
  * @param  None
  * @retval None
  */
static void TSENSOR_SetHint(void)
{
  /* Clear the LCD */ 
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  
  /* Set LCD Demo description */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), 80);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE); 
  BSP_LCD_SetFont(&Font24);
  BSP_LCD_DisplayStringAt(0, 0, (uint8_t*)"TEMPERATURE SENSOR", CENTER_MODE);
  BSP_LCD_SetFont(&Font12);
  BSP_LCD_DisplayStringAt(0, 30, (uint8_t*)"This example shows how to", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 45, (uint8_t*)"read a Temperature", CENTER_MODE); 
  BSP_LCD_DisplayStringAt(0, 60, (uint8_t*)"and manage temperature limit alert", CENTER_MODE); 

   /* Set the LCD Text Color */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);  
  BSP_LCD_DrawRect(10, 90, BSP_LCD_GetXSize() - 20, BSP_LCD_GetYSize()- 100);
  BSP_LCD_DrawRect(11, 91, BSP_LCD_GetXSize() - 22, BSP_LCD_GetYSize()- 102);

  /* Prepare LCD to display */
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE);  
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);    
  BSP_LCD_FillRect(12, 92, BSP_LCD_GetXSize() - 24, BSP_LCD_GetYSize()- 104);
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
}

  /**
  * @brief  Display temperature
  * @param  temperature : temperature value
  * @retval None
  */
static void TSENSOR_Display_Temperature(uint16_t Temperature)
{
  uint8_t tempcelsiusdisplay[] = "+abc.dddd C";
  uint16_t temperaturevalue = Temperature;
  uint16_t temperaturevaluecelsius = 0;
  uint32_t tempcelsius = 0;

  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_SetFont(&Font12);
  BSP_LCD_DisplayStringAt(0, 115, (uint8_t*)"Measured Temperature : ", CENTER_MODE);

  /* Change Font size to display Temperature Value */
  BSP_LCD_SetFont(&Font16);

  /* Verify the sign of the temperature */
  if (temperaturevalue <= 2048)
  {
    /* Positive temperature measured */
    tempcelsiusdisplay[0] = '+';
    /* Initialize the temperature sensor value*/
    temperaturevaluecelsius = temperaturevalue;
    /* Set Text color to Green */
    BSP_LCD_SetTextColor(LCD_COLOR_GREEN);    
  }
  else
  {
    /* Negative temperature measured */
    tempcelsiusdisplay[0] = '-';
    /* Remove temperature value sign */
    temperaturevaluecelsius = 0x1000 - temperaturevalue;
    /* Set Text color to Red */
    BSP_LCD_SetTextColor(LCD_COLOR_BLUE);  
  }

  tempcelsius = 0;
  /* Calculate temperature digits in °C */
  if (temperaturevaluecelsius & 0x01)
  {
    tempcelsius += 625;     
  }
  if (temperaturevaluecelsius & 0x02)
  {
    tempcelsius += 1250;
  }
  if (temperaturevaluecelsius & 0x04)
  {
    tempcelsius += 2500;
  }
  if (temperaturevaluecelsius & 0x08)
  {
    tempcelsius += 5000;
  }
  
  tempcelsiusdisplay[5] = (tempcelsius / 1000) + 0x30;
  tempcelsiusdisplay[6] = ((tempcelsius % 1000) / 100) + 0x30;
  tempcelsiusdisplay[7] = (((tempcelsius % 1000) % 100) / 10)+ 0x30;
  tempcelsiusdisplay[8] = (((tempcelsius % 1000) % 100) % 10) + 0x30;
    
  temperaturevaluecelsius >>= 4;

  tempcelsiusdisplay[1] = (temperaturevaluecelsius / 100) + 0x30;
  tempcelsiusdisplay[2] = ((temperaturevaluecelsius % 100) / 10) + 0x30;
  tempcelsiusdisplay[3] = ((temperaturevaluecelsius % 100) % 10) + 0x30;

  /* Display Temperature value on LCD */
  BSP_LCD_DisplayStringAt(0, 145, tempcelsiusdisplay, CENTER_MODE); 
}

/**
  * @brief  TSENSOR error callbacks.
  * @param  Error : Temperature Sensor Error status.
  * @retval None
  */
void TSENSOR_ErrorCallback(uint16_t Error)
{
  if (Error == TSENSOR_ALERT)
  {
    alertoccurs = 1;
  }
  else
  {
    Error_Handler();
  }
  
}
/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
    /* Clear Previous message on the LCD */
    BSP_LCD_SetBackColor(LCD_COLOR_WHITE);  
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);    
    BSP_LCD_FillRect(12, 92, BSP_LCD_GetXSize() - 24, BSP_LCD_GetYSize()- 104);

    /* Display Communication error message */
    BSP_LCD_SetTextColor(LCD_COLOR_RED);    
    BSP_LCD_DisplayStringAt(0, 115, (uint8_t*)"An error occurs during", CENTER_MODE); 
    BSP_LCD_DisplayStringAt(0, 130, (uint8_t*)"communication with", CENTER_MODE);
    BSP_LCD_DisplayStringAt(0, 145, (uint8_t*)"the temperature sensor", CENTER_MODE);

    /* Turn LED_RED on */
    BSP_LED_On(LED_RED);
    while(1)
    {
    }
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */ 

/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
