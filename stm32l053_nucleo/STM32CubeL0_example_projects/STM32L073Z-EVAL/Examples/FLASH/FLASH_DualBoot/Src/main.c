/**
  ******************************************************************************
  * @file    FLASH/FLASH_DualBoot/Src/main.c 
  * @author  MCD Application Team
  * @brief   This example provides a description of how to boot from bank1 or bank2
  *          of the STM32L0xx FLASH.
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

/** @addtogroup STM32L0xx_HAL_Examples
  * @{
  */

/** @addtogroup FLASH_DualBoot
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define MESSAGE1     "STM32L073VZ" 
#define MESSAGE2     "Flash Dual-boot example"
#define MESSAGE3     "Device running on" 

#ifdef FLASH_BANK1
 #define MESSAGE4    "FLASH BANK1"
#else
 #define MESSAGE4    "FLASH BANK2"
#endif

#define MESSAGE5     "PUSH BUTTON_TAMPER"
#define MESSAGE6     "to swap bank"

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
FLASH_AdvOBProgramInitTypeDef    AdvOBInit; 
/* uint32_t primask_cur; */

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void Display_ExampleDescription(void);
static void FLASH_Unlock(void);
 
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
#if defined(STM32L071xx) || defined(STM32L081xx) 
/*primask_cur = __get_PRIMASK();*/
  
/*Reset the Primask register to unmask interrupts*/
__set_PRIMASK(0x00000000);
#endif

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

  FLASH_Unlock();  /* uncomment if Flash locked to change settings*/
  __HAL_RCC_SYSCFG_CLK_ENABLE();
  
  /* Configure BUTTON_TAMPER */
  BSP_PB_Init(BUTTON_TAMPER, BUTTON_MODE_GPIO);

  /* Initialize LED1, LED2 and LED3 */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED2);
  BSP_LED_Init(LED3);

  Display_ExampleDescription();
  
  /* Turn on LEDs */
  BSP_LED_On(LED1);
  BSP_LED_On(LED2);

  /*--- If Wake-up button is pushed, Set or reset BFB2 bit to enable or disbale 
  boot from Bank2 (active after next reset, w/ Boot pins set in Boot from Flash 
  memory position ---*/

  while (1)
  {
    /* Wait for BUTTON_TAMPER is released */
    if (BSP_PB_GetState(BUTTON_TAMPER) == SET)
    {
      while (BSP_PB_GetState(BUTTON_TAMPER) == SET);

      /* Set BFB2 bit to enable boot from Flash Bank2 */
      /* Allow Access to Flash control registers and user Flash */
      HAL_FLASH_Unlock();
      
      /* Clear OPTVERR bit set on virgin samples */
      __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR); 

      /* Allow Access to option bytes sector */ 
      HAL_FLASH_OB_Unlock();
      
      /* Get the Dual boot configuration status */
      AdvOBInit.OptionType = OPTIONBYTE_BOOTCONFIG;
      HAL_FLASHEx_AdvOBGetConfig(&AdvOBInit);
  
      /* Switch Boot bank */
      if (AdvOBInit.BootConfig == OB_BOOT_BANK2)
      {
        /* At startup, if boot pin 0 and BOOT1 bit are set in boot from user Flash position
           and this parameter is selected the device will boot from Bank 1 */
        AdvOBInit.BootConfig = OB_BOOT_BANK1;
      }
      else
      {
        /* At startup, if boot pin 0 and BOOT1 bit are set in boot from user Flash position
           and this parameter is selected the device will boot from Bank 2 */
        AdvOBInit.BootConfig = OB_BOOT_BANK2;
      }

      if(HAL_FLASHEx_AdvOBProgram (&AdvOBInit) != HAL_OK)
      {
        /*
        Error occurred while setting option bytes configuration.
        User can add here some code to deal with this error.
        To know the code error, user can call function 'HAL_FLASH_GetError()'
        */
        /* Infinite loop */
        while (1)
        {
          BSP_LED_On(LED3);
        }
      }
      
      /* Start the Option Bytes programming process */  
      if (HAL_FLASH_OB_Launch() != HAL_OK)
      {
        /*
        Error occurred while reloading option bytes configuration.
        User can add here some code to deal with this error.
        To know the code error, user can call function 'HAL_FLASH_GetError()'
        */
        /* Infinite loop */
        while (1)
        {
          BSP_LED_On(LED3);
        }
      }
    }
    else
    {
#ifdef FLASH_BANK1        
      /* Toggle LED1 */
      BSP_LED_Toggle(LED1);
#else 
      /* Toggle LED2 */    
      BSP_LED_Toggle(LED2);
#endif    
      
      /* Insert 50 ms delay */
      HAL_Delay(50);
    }
  }
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSI)
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLSource   = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLState    = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLMUL      = RCC_PLL_MUL4;
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
  * @brief  Display main example messages
  * @param  None
  * @retval None
  */
static void Display_ExampleDescription(void)
{
  /* Initialize the LCD */
  BSP_LCD_Init();

  /* Clear the LCD */ 
  BSP_LCD_Clear(LCD_COLOR_WHITE);

#ifdef FLASH_BANK1
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
#else
  BSP_LCD_SetTextColor(LCD_COLOR_RED);
#endif /* BOOT_FROM_BANK1 */
  BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), 80);

  
  /* Display LCD messages */
#ifdef FLASH_BANK1
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
#else
  BSP_LCD_SetBackColor(LCD_COLOR_RED);
#endif /* BOOT_FROM_BANK1 */
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);

  BSP_LCD_SetFont(&Font24);
  BSP_LCD_DisplayStringAt(0, 10, (uint8_t *)MESSAGE1, CENTER_MODE);

  BSP_LCD_SetFont(&Font16);
  BSP_LCD_DisplayStringAt(0, 40, (uint8_t *)MESSAGE2, CENTER_MODE);

#ifdef FLASH_BANK1
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
#else
  BSP_LCD_SetTextColor(LCD_COLOR_RED);
#endif /* BOOT_FROM_BANK1 */
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE);

  BSP_LCD_SetFont(&Font12);
  BSP_LCD_DisplayStringAt(0, 95, (uint8_t *)MESSAGE3, CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 110, (uint8_t *)MESSAGE4, CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 140, (uint8_t *)MESSAGE5, CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 155, (uint8_t *)MESSAGE6, CENTER_MODE);
}

static void FLASH_Unlock(void)  
{
  if((FLASH->PECR & FLASH_PECR_PRGLOCK) != RESET)
  {
    /* Unlocking FLASH_PECR register access */
    if((FLASH->PECR & FLASH_PECR_PELOCK) != RESET)
     {  
         FLASH->PEKEYR = FLASH_PEKEY1;
         FLASH->PEKEYR = FLASH_PEKEY2;
     }
 
    /* Unlocking the program memory access */
    FLASH->PRGKEYR = FLASH_PRGKEY1;
    FLASH->PRGKEYR = FLASH_PRGKEY2;  
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
