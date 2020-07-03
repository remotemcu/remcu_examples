/**
  ******************************************************************************
  * @file    main.c
  * @author  MCD Application Team
  * @brief   This file provides main program functions
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2016 STMicroelectronics International N.V. All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice,
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other
  *    contributors to this software may be used to endorse or promote products
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under
  *    this license is void and will automatically terminate your rights under
  *    this license.
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "k_config.h"
#include "main.h"
#include "k_window.h"
#include "k_demo.h"
#include "k_menu.h"
#include "k_module.h"
#include "k_storage.h"
#include "main_app.h"
#include "thermometer_app.h"
#include "lpuart_wakeup_app.h"
#include "lowpower_app.h"
#include "pressure_app.h"
#include "lc_sensor_metering_app.h"
#include "help_app.h"


/* Shared variable used to manage the tamper button */
volatile FlagStatus FlagItTamperButton = RESET;
UART_HandleTypeDef UartHandle_1;


/* global Functions */
static void Error_Handler(void);
static void  RtcClockSetup(rtcConfigClockEnum flag);

/* static Functions */
void SystemClock_Config(void);
void SystemClock_ConfigMSI_Slow(void);

/**
* @}
*/

/** @defgroup MAIN_Private_Functions
* @{
*/

/**
* @brief  Main program
* @param  None
* @retval int
*/



extern void ManageIddMeasurementMenu(char * messTitle,uint32_t flagModeIt);
extern void DisplayStandbyIddMeasurement(void);

int main(void)
{

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock @ CPU max */
  SystemClock_Config();

   /* Set the SysTick to the highest priority in order to avoid
   * being blocked inside functions waiting on timeouts.
   */
   HAL_InitTick(0);

  /* LED init */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED2);
  BSP_LED_Init(LED3);
  BSP_LED_Init(LED4);

  HAL_NVIC_SetPriority(EXTI4_15_IRQn, 1, 0);
  /* Reset Idd measurement component if not reseting from standby mode */
  if (__HAL_PWR_GET_FLAG(PWR_FLAG_SB) == RESET)
  {
    BSP_IDD_Reset();
  }

  BSP_IDD_Init();

  BSP_IDD_ClearIT();
  BSP_IDD_ErrorClearIT();

  BSP_IDD_DisableIT();
  BSP_IDD_ErrorDisableIT();


  /* Start the demo thread */
  kDemo_Start();
  return 0;
}

/* DecodeMFXInterrupt                                  */
/* Check which interrupt has been detected through MFX */

/** @brief Check which interrupt has been detected through MFX
  * @param ptJoyState: joystick state update through this function
  * @param ptIddState: idd state update through this function
  * @retval None
  */
void DecodeMFXInterrupt(uint32_t *ptJoyState, uint32_t *ptIddState)
{
  uint32_t itStatus = 0;

  /*Initialize the states*/
  *ptJoyState = JOY_NONE_PIN;
  *ptIddState = IDD_STATE_UNCHANGED;

   /* Check if it was an event on the Joystick */
   /********************************************/
   itStatus = BSP_IO_ITGetStatus(JOY_ALL_PINS);
   if (itStatus != 0) // The joystick has been selected...
   {
     /* Define the GPIO which was selected */
     if ((itStatus & JOY_SEL_PIN) == JOY_SEL_PIN)
     {
         *ptJoyState =JOY_SEL_PIN;
     }
     else if ((itStatus & JOY_DOWN_PIN) == JOY_DOWN_PIN)
     {
         *ptJoyState =JOY_DOWN_PIN;
     }
     else if ((itStatus & JOY_LEFT_PIN) == JOY_LEFT_PIN)
     {
         *ptJoyState =JOY_LEFT_PIN;
     }
     else if ((itStatus & JOY_RIGHT_PIN) == JOY_RIGHT_PIN)
     {
         *ptJoyState =JOY_RIGHT_PIN;
     }
     else if ((itStatus & JOY_UP_PIN) == JOY_UP_PIN)
     {
         *ptJoyState =JOY_UP_PIN;
     }
     else
     {
       *ptJoyState =JOY_NONE_PIN;
     }
     /* Clear the IT */
     BSP_IO_ITClear();
   }

   /* Note : Since the IDD interupts are no more used, it is not necessary
    * to check for the IDD_Termination_Measurement interrupt
    * and the IDD_Error interrupt
    */
}

/** @brief EXTI line detection callbacks
  * @param GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  uint32_t JoyState_tmp = 0;
  uint32_t IddState_tmp = 0;

  switch(GPIO_Pin)
  {
  case SD_DETECT_PIN :
    kStorage_SdDetection(BSP_SD_IsDetected());
    break;
  case MFX_IRQOUT_PIN :
    /* In this case, only a flag is set.
     * Note : To get the origin of this interrupt, it is requested to send I2C commands
     * to the MFX, but this cannot be done inside the Interrupt Handler.
     */

    DecodeMFXInterrupt(&JoyState_tmp, &IddState_tmp);

    if (IddState_tmp==IDD_STATE_UNCHANGED){ /* only for Joystick IT case*/
      kMenu_EventHandler(JoyState_tmp);
    }

    break;
  case TAMPER_BUTTON_PIN :
     FlagItTamperButton = SET;
    break;

  default :
    Error_Handler();
    break;
  }

}

/** @brief EXTI line detection callbacks
  * @param GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void kDemo_Initialization(void)
{
  /* Initialize the LCD */
  BSP_LCD_Init();

  if (__HAL_PWR_GET_FLAG(PWR_FLAG_SB) != RESET)
  {
      DisplayStandbyIddMeasurement();

    /* Clear StandBy flag */
    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_SB);
  }

  BSP_LCD_Clear(LCD_COLOR_WHITE);

  /* Check the SD presence else wait until SD insertion */
  BSP_SD_Init();

  if(BSP_SD_IsDetected() != SD_PRESENT)
  {
    kWindow_Error("Sd card not \n present\n...\n\nWait sd insertion\n");
    HAL_Delay(2000); /* Wait for 2 sec */
  }

  /* FatFs initialisation */
  if(kStorage_Init() != KSTORAGE_NOERROR)
  {
    kWindow_Error("FatFs initialization failed\nplease check the SD card\n");
    HAL_Delay(2000); /* Wait for 2 sec */
  }

  /* Display the demonstration window */
  BSP_LCD_SetFont(&Font24);
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), Font24.Height*2);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_DisplayStringAt( 0,              0,      (uint8_t *)EVAL_BOARD, CENTER_MODE);
  BSP_LCD_DisplayStringAt( 0,  Font24.Height, (uint8_t *)"DEMONSTRATION", CENTER_MODE);
  kStorage_OpenFileDrawPixel(80, 90, (uint8_t *)"STFILES/STLogo.bmp");

  /* Let time user to see information */
  HAL_Delay(2000);

  /* Module Initialization */
  kModule_Init();
}

void kModule_Init(void)
{
   kModule_Add(MODULE_MAIN_APP, ModuleAppMain);
   kModule_Add(MODULE_LPUART_WAKEUP, LpUartWakepModuleItem);
   kModule_Add(MODULE_LOW_POWER, LowPowerModuleItem);
   kModule_Add(MODULE_TSENSOR , ModuleTsensor);
   kModule_Add(MODULE_LC_SENSOR_METERING , LcSensorMeteringModuleItem);
   kModule_Add(MODULE_PRESSURE_MEASUREMENT , PressureModuleItem);
   kModule_Add(MODULE_SYSTEM_INFO , HelpModuleItem);

}

void kDemo_UnInitialization(void)
{
  /* Nothing to do */
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

  /* The voltage scaling allows optimizing the power consumption when the device is
     clocked below the maximum system frequency, to update the voltage scaling value
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = HSI
  *            SYSCLK(Hz)                     = 16000000
  *            HCLK(Hz)                       = 16000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            VDD(V)                         = 3.3
  * @param  None
  * @retval None
  */
void SystemClock_ConfigHSI_noPLL_16M(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  /* Make sure we can select the highest Clock and the fastest Flash access */
  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();

 /* VCore set to 1,8V */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /* Enable HSI as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue= RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct)!= HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }
  /* Select HSI as system clock source and configure the HCLK, PCLK1 and PCLK2
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    while(1);
  }

  /* Disable HSE and MSI if requested ... */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.HSEState = RCC_HSE_OFF;
  RCC_OscInitStruct.MSIState = RCC_MSI_OFF;
  RCC_OscInitStruct.MSICalibrationValue = 0x00;
  RCC_OscInitStruct.PLL.PLLState    = RCC_PLL_OFF;

  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
}

/**
  * @brief  System Clock Configuration for low power tests
  *         The system Clock is configured as follow :
  *            System Clock source            = MSI
  *            SYSCLK(Hz)                     = 65.536 KHz
  *            HCLK(Hz)                       = 65.536 KHz
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            APB2 Prescaler                 = 1
  *            Flash Latency(WS)              = 0
  *            Main regulator output voltage  = Scale2 mode
  * @retval None
  */
void SystemClock_Config_ForLowPower(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};

  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();


  /* Enable MSI Oscillator */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_0;
  RCC_OscInitStruct.MSICalibrationValue=0x00;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct)!= HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }

  /* Select MSI as system clock source and configure the HCLK, PCLK1 and PCLK2
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0)!= HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }

  /* The voltage scaling allows optimizing the power consumption when the device is
     clocked below the maximum system frequency, to update the voltage scaling value
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = (MSI)
  *            MSI Range                      = 2
  *            SYSCLK(Hz)                     = 64000
  *            HCLK(Hz)                       = 32000
  *            AHB Prescaler                  = 2
  *            APB1 Prescaler                 = 1
  *            APB2 Prescaler                 = 1
  *            Main regulator output voltage  = Scale2 mode
  * @param  None
  * @retval None
  */

void SystemClock_ConfigMSI(void)
{
 RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();

  /* The voltage scaling allows optimizing the power consumption when the device is
     clocked below the maximum system frequency, to update the voltage scaling value
     regarding system frequency refer to product datasheet.
     Low power sleep mode can only be entered when VCORE is in range 2. */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /* Enable MSI Oscillator */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_5;
  RCC_OscInitStruct.MSICalibrationValue = 0x00;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct)!= HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }

  /* Select MSI as system clock source and configure the HCLK, PCLK1 and PCLK2
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0)!= HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }

  /* Disable the previous clock : System Clock source PLL (HSE) */

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_OFF;
  RCC_OscInitStruct.PLL.PLLSource   = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLState    = RCC_PLL_OFF;

  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /* Disable the previous clock : System Clock source HSI*/

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSEState = RCC_HSI_OFF;
  RCC_OscInitStruct.PLL.PLLSource   = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLState    = RCC_PLL_OFF;

  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /* Set MSI range to 0 */
  __HAL_RCC_MSI_RANGE_CONFIG(RCC_MSIRANGE_0);

  /* Configure the SysTick to have interrupt in 10 ms time basis*/
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/100);
}

/**
  * @brief  SystemClock_DisableMSI
  *         Disable the MSI clock
  * @param  None
  * @retval None
  */

void SystemClock_DisableMSI(void)
{


  RCC_OscInitTypeDef RCC_OscInitStruct = {0};

  /* Disable the previous clock : System Clock source MSI */

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_OFF;
  RCC_OscInitStruct.PLL.PLLState    = RCC_PLL_NONE;

  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
      Error_Handler();
  }
  /* Set the default value */
  __HAL_RCC_MSI_RANGE_CONFIG(RCC_MSIRANGE_5);

}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = (MSI)
  *            MSI Range                      = 2
  *            SYSCLK(Hz)                     = 64000
  *            HCLK(Hz)                       = 32000
  *            AHB Prescaler                  = 2
  *            APB1 Prescaler                 = 1
  *            APB2 Prescaler                 = 1
  *            Main regulator output voltage  = Scale2 mode
  * @param  None
  * @retval None
  */
void SystemClock_ConfigMSI_Slow(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();

  /* The voltage scaling allows optimizing the power consumption when the device is
     clocked below the maximum system frequency, to update the voltage scaling value
     regarding system frequency refer to product datasheet.
     Low power sleep mode can only be entered when VCORE is in range 2. */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /* Enable MSI Oscillator */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_5;
  RCC_OscInitStruct.MSICalibrationValue = 0x00;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct)!= HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }

  /* Select MSI as system clock source and configure the HCLK, PCLK1 and PCLK2
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0)!= HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }

  /* Set MSI range to 0 */
  __HAL_RCC_MSI_RANGE_CONFIG(RCC_MSIRANGE_0);

  /* Configure the SysTick to have interrupt in 10 ms time basis*/
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/100);
}

/**
  * @brief Error handler
  * @param None:
  * @retval None
  */
void Error_Handler(void)
{
  while(1)
  {
    HAL_Delay(100);
    BSP_LED_Toggle(LED1);
    BSP_LED_Toggle(LED2);
    BSP_LED_Toggle(LED3);
    BSP_LED_Toggle(LED4);
  }
}

/**
  * @brief SMBUS MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  *           - NVIC configuration for DMA interrupt request enable
  * @param hsmbus: SMBUS handle pointer
  * @retval None
  */
void HAL_SMBUS_MspInit(SMBUS_HandleTypeDef *hsmbus)
{
  GPIO_InitTypeDef  GPIO_InitStruct;
  RCC_PeriphCLKInitTypeDef  RCC_PeriphCLKInitStruct;

  /*##-1- Configure the SMBUS clock source. The clock is derived from the SYSCLK #*/
  RCC_PeriphCLKInitStruct.PeriphClockSelection = SMBUSx_RCC_PERIPHCLK;
  RCC_PeriphCLKInitStruct.I2c1ClockSelection = SMBUSx_CLKSOURCE_SYSCLK;
  HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphCLKInitStruct);

  /*##-2- Enable peripherals and GPIO Clocks #################################*/
  /* Enable GPIO TX/RX clock */
  SMBUSx_SCL_GPIO_CLK_ENABLE();
  SMBUSx_SDA_GPIO_CLK_ENABLE();

  /* Enable SMBUSx clock */
  SMBUSx_CLK_ENABLE();

  /*##-3- Configure peripheral GPIO ##########################################*/
  /* SMBUS TX GPIO pin configuration  */
  GPIO_InitStruct.Pin              = SMBUSx_SCL_PIN;
  GPIO_InitStruct.Mode             = GPIO_MODE_AF_OD;
  GPIO_InitStruct.Pull             = GPIO_PULLUP;
  GPIO_InitStruct.Speed            = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate        = SMBUSx_SCL_SDA_AF;

  HAL_GPIO_Init(SMBUSx_SCL_GPIO_PORT, &GPIO_InitStruct);

  /* SMBUS RX GPIO pin configuration  */
  GPIO_InitStruct.Pin              = SMBUSx_SDA_PIN;
  GPIO_InitStruct.Alternate        = SMBUSx_SCL_SDA_AF;

  HAL_GPIO_Init(SMBUSx_SDA_GPIO_PORT, &GPIO_InitStruct);

  /* SMBUS ALERT GPIO pin configuration  */
  GPIO_InitStruct.Pin              = SMBUSx_ALERT_PIN;
  GPIO_InitStruct.Alternate        = SMBUSx_ALERT_AF;

  HAL_GPIO_Init(SMBUSx_ALERT_GPIO_PORT, &GPIO_InitStruct);

  /*##-4- Configure the NVIC for SMBUS ########################################*/
  /* NVIC for SMBUSx */
  HAL_NVIC_SetPriority(SMBUSx_IRQn, IT_PRIO_SMBUS, 0);
  HAL_NVIC_EnableIRQ(SMBUSx_IRQn);
}

/**
  * @brief SMBUS MSP De-Initialization
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO, DMA and NVIC configuration to their default state
  * @param hsmbus: SMBUS handle pointer
  * @retval None
  */
void HAL_SMBUS_MspDeInit(SMBUS_HandleTypeDef *hsmbus)
{
  /*Disable the NVIC for SMBUS */
  HAL_NVIC_DisableIRQ(SMBUSx_IRQn);
}

/**
  * @brief UART MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  *           - DMA configuration for transmission request by peripheral
  *           - NVIC configuration for DMA interrupt request enable
  * @param huart: UART handle pointer
  * @retval None
  */
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{

  if(huart->Instance  == LPUART1) {

  static DMA_HandleTypeDef hdma_tx;
  static DMA_HandleTypeDef hdma_rx;

  GPIO_InitTypeDef  GPIO_InitStruct;

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;


  /*##- Configure LSE for LPUART ###################################*/
  HAL_RCC_GetOscConfig(&RCC_OscInitStruct);

  RCC_OscInitStruct.OscillatorType =  RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  RCC_OscInitStruct.LSIState = RCC_LSI_OFF;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LPUART1;
  PeriphClkInitStruct.Lpuart1ClockSelection = RCC_LPUART1CLKSOURCE_LSE;
  if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }


  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* Enable GPIO clock */
  USARTx_TX_GPIO_CLK_ENABLE();
  USARTx_RX_GPIO_CLK_ENABLE();

  /* Enable USARTx clock */
  USARTx_CLK_ENABLE(); //__HAL_RCC_LPUART1_CLK_ENABLE();

  /* Enable DMA clock */
  DMAx_CLK_ENABLE();


  /*##-2- Configure peripheral GPIO ##########################################*/
  /* UART TX GPIO pin configuration  */
  GPIO_InitStruct.Pin       = USARTx_TX_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_NOPULL; //GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = USARTx_TX_AF;

  HAL_GPIO_Init(USARTx_TX_GPIO_PORT, &GPIO_InitStruct);

  /* UART RX GPIO pin configuration  */
  GPIO_InitStruct.Pin = USARTx_RX_PIN;
  GPIO_InitStruct.Alternate = USARTx_RX_AF;

  HAL_GPIO_Init(USARTx_RX_GPIO_PORT, &GPIO_InitStruct);

  /*##-3- Configure the DMA ##################################################*/
  /* Configure the DMA handler for Transmission process */
  hdma_tx.Instance                 = USARTx_TX_DMA_CHANNEL;
  hdma_tx.Init.Direction           = DMA_MEMORY_TO_PERIPH;
  hdma_tx.Init.PeriphInc           = DMA_PINC_DISABLE;
  hdma_tx.Init.MemInc              = DMA_MINC_ENABLE;
  hdma_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
  hdma_tx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
  hdma_tx.Init.Mode                = DMA_NORMAL;
  hdma_tx.Init.Priority            = DMA_PRIORITY_LOW;
  hdma_tx.Init.Request             = USARTx_TX_DMA_REQUEST;

  HAL_DMA_Init(&hdma_tx);

  /* Associate the initialized DMA handle to the UART handle */
  __HAL_LINKDMA(huart, hdmatx, hdma_tx);

  /* Configure the DMA handler for reception process */
  hdma_rx.Instance                 = USARTx_RX_DMA_CHANNEL;
  hdma_rx.Init.Direction           = DMA_PERIPH_TO_MEMORY;
  hdma_rx.Init.PeriphInc           = DMA_PINC_DISABLE;
  hdma_rx.Init.MemInc              = DMA_MINC_ENABLE;
  hdma_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
  hdma_rx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
  hdma_rx.Init.Mode                = DMA_NORMAL;
  hdma_rx.Init.Priority            = DMA_PRIORITY_HIGH;
  hdma_rx.Init.Request             = USARTx_RX_DMA_REQUEST;

  HAL_DMA_Init(&hdma_rx);

  /* Associate the initialized DMA handle to the the UART handle */
  __HAL_LINKDMA(huart, hdmarx, hdma_rx);

  /*##-4- Configure the NVIC for DMA #########################################*/
  /* NVIC configuration for DMA transfer complete interrupt (USARTx_TX and USARTx_RX) */
  HAL_NVIC_SetPriority(USARTx_DMA_TX_RX_IRQn, IT_PRIO_DMA_LPUART1, 0);
  HAL_NVIC_EnableIRQ(USARTx_DMA_TX_RX_IRQn);

  /* NVIC for USART, to catch the TX complete */
  HAL_NVIC_SetPriority(USARTx_IRQn, IT_PRIO_LPUART1, 0);
  HAL_NVIC_EnableIRQ(USARTx_IRQn);

  }
}

/**
  * @brief UART MSP De-Initialization
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO, DMA and NVIC configuration to their default state
  * @param huart: UART handle pointer
  * @retval None
  */
void HAL_UART_MspDeInit(UART_HandleTypeDef *huart)
{
  if(huart->Instance  == LPUART1) {

  static DMA_HandleTypeDef hdma_tx;
  static DMA_HandleTypeDef hdma_rx;

  /*##-1- Reset peripherals ##################################################*/
  USARTx_FORCE_RESET();
  USARTx_RELEASE_RESET();


  /*##-2- Disable peripherals and GPIO Clocks #################################*/
  /* Configure UART Tx as alternate function  */
  HAL_GPIO_DeInit(USARTx_TX_GPIO_PORT, USARTx_TX_PIN);
  /* Configure UART Rx as alternate function  */
  HAL_GPIO_DeInit(USARTx_RX_GPIO_PORT, USARTx_RX_PIN);

  /*##-3- Disable the DMA Channels ###########################################*/
  /* De-Initialize the DMA Channel associated to transmission process */
  HAL_DMA_DeInit(&hdma_tx);
  /* De-Initialize the DMA Channel associated to reception process */
  HAL_DMA_DeInit(&hdma_rx);

  /*##-4- Disable the NVIC for DMA ###########################################*/
  HAL_NVIC_DisableIRQ(USARTx_DMA_TX_RX_IRQn);

  /*##-5- Disable the different clocks  ###########################################*/
  __HAL_RCC_LPUART1_CLK_DISABLE();
  __HAL_RCC_DMA1_CLK_DISABLE();

  }
}

/**
  * @brief Configure the Rtc clock
  * @param flag: (RCC_LSE_ON by default)
  * @retval None
  */
static void  RtcClockSetup(rtcConfigClockEnum flag)
{
  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;


  switch(flag)
  {
    case RTC_CLOCK_NONE :   RCC_OscInitStruct.OscillatorType =  RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_LSE;
                            RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
                            RCC_OscInitStruct.LSIState = RCC_LSI_OFF;
                            RCC_OscInitStruct.LSEState = RCC_LSE_OFF;
                            if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
                            {
                                while(1)
                                  {
                                   }
                            }
                            __HAL_RCC_RTC_DISABLE();
                            break;

    case RTC_CLOCK_LSI :    RCC_OscInitStruct.OscillatorType =  RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_LSE;
                            RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
                            RCC_OscInitStruct.LSIState = RCC_LSI_ON;
                            RCC_OscInitStruct.LSEState = RCC_LSE_OFF;
                            if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
                            {
                                while(1)
                                {
                                }
                            }
                            PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
                            PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
                            if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
                            {
                                 while(1)
                                  {
                                  }
                             }
                            __HAL_RCC_RTC_ENABLE();
                            break;

   case RTC_CLOCK_LSE :     RCC_OscInitStruct.OscillatorType =  RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_LSE;
                            RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
                            RCC_OscInitStruct.LSIState = RCC_LSI_OFF;
                            RCC_OscInitStruct.LSEState = RCC_LSE_ON;
                            if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
                            {
                                while(1)
                                {
                                }
                            }
                            PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
                            PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
                            if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
                            {
                                 while(1)
                                  {
                                  }
                             }
                            __HAL_RCC_RTC_ENABLE();
                            break;
   default            :     break;

  }

}

/**
  * @brief RTC MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  * @param hrtc: RTC handle pointer
  * @note  Care must be taken when HAL_RCCEx_PeriphCLKConfig() is used to select
  *        the RTC clock source; in this case the Backup domain will be reset in
  *        order to modify the RTC Clock source, as consequence RTC registers (including
  *        the backup registers) and RCC_CSR register are set to their reset values.
  * @retval None
  */
void HAL_RTC_MspInit(RTC_HandleTypeDef *hrtc)
{


   RtcClockSetup(RTC_CLOCK_LSE);

  /* Configure the NVIC for RTC Alarm */
  HAL_NVIC_SetPriority(RTC_IRQn, IT_PRIO_RTC, 0);

  /* Enable the RTC global Interrupt */
  HAL_NVIC_EnableIRQ(RTC_IRQn);
}

/**
  * @brief RTC MSP De-Initialization
  *        This function freeze the hardware resources used in this example:
  *        by disabling the Peripheral's clock
  * @param hrtc: RTC handle pointer
  * @retval None
  */
void HAL_RTC_MspDeInit(RTC_HandleTypeDef *hrtc)
{


     RtcClockSetup(RTC_CLOCK_NONE);
}

#ifdef USE_FULL_ASSERT
/**
* @brief  assert_failed
*         Reports the name of the source file and the source line number
*         where the assert_param error has occurred.
* @param  File: pointer to the source file name
* @param  Line: assert_param error line source number
* @retval None
*/
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line
  number,ex: printf("Wrong parameters value: file %s on line %d\r\n",
  file, line) */

  /* Infinite loop */
  while (1)
  {}
}

#endif

/**
* @}
*/

/**
* @}
*/

/**
* @}
*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
