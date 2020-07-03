/**
  ******************************************************************************
  * @file    LCD/LCD_Blink_Frequency/Src/main.c 
  * @author  MCD Application Team
  * @brief   This example describes how to configure and use GPIOs through 
  *          the STM32L0xx HAL API.
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

/** @addtogroup LCD_Blink_Frequency
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern LCD_HandleTypeDef LCDHandle;
uint32_t BlinkCounter = 1; 
/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /* This sample code shows how to use STM32L0xx LCD HAL API to configures the LCD Blink
     mode and Blink frequency.*/  
  
  /* STM32L0xx HAL library initialization:
       - Configure the Flash prefetch, Flash preread and Buffer caches
       - Systick timer is configured by default as source of time base, but user 
             can eventually implement his proper time base source (a general purpose 
             timer for example or other time source), keeping in mind that Time base 
             duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and 
             handled in milliseconds basis.
       - Low Level Initialization
     */
  HAL_Init();

  /* Configure the KEY Push button */
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI);
    
  /* Configure the system clock */
  SystemClock_Config();
   
  /* LCD GLASS Initialization */
  BSP_LCD_GLASS_Init();
  
  /* Configure all LCD segments and Coms to blink at frequency LCDCLK/16 */
  __HAL_LCD_BLINK_CONFIG(&LCDHandle, LCD_BLINKMODE_ALLSEG_ALLCOM, LCD_BLINKFREQUENCY_DIV16);
  
  /* Display the "BK0016" message on the LCD GLASS */
  BSP_LCD_GLASS_DisplayString((uint8_t*)"BK0016");
  
  /* Infinite loop */
  while (1)
  {    
  }
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = MSI
  *            SYSCLK(Hz)                     = 2000000
  *            HCLK(Hz)                       = 2000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            APB2 Prescaler                 = 1
  *            Flash Latency(WS)              = 0
  *            Main regulator output voltage  = Scale3 mode
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  
  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();
  
  /* The voltage scaling allows optimizing the power consumption when the device is 
     clocked below the maximum system frequency, to update the voltage scaling value 
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);
  
  /* Enable MSI Oscillator */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_5;
  RCC_OscInitStruct.MSICalibrationValue=0x00;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);
  
  
  /* Select MSI as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;  
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0);
  
}

/**
  * @brief EXTI line detection callbacks
  * @param GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if(GPIO_Pin == KEY_BUTTON_PIN)
  {
    /* Clear The LCD GLASS */
    BSP_LCD_GLASS_Clear();
    
    switch(BlinkCounter)
    {
    case 0:
      /* Configure all LCD segments and Coms to blink at frequency LCDCLK/16 */      
      __HAL_LCD_BLINK_CONFIG(&LCDHandle, LCD_BLINKMODE_ALLSEG_ALLCOM, LCD_BLINKFREQUENCY_DIV16);
      
      /* Display the "BK0016" message on the LCD GLASS */
      BSP_LCD_GLASS_DisplayString((uint8_t*)"BK0016");
      
      /* Increment the Blink freqency */
      BlinkCounter++;
      break;
      
    case  1: 
      /* Configure all LCD segments and Coms to blink at frequency LCDCLK/32 */ 
      __HAL_LCD_BLINK_CONFIG(&LCDHandle, LCD_BLINKMODE_ALLSEG_ALLCOM, LCD_BLINKFREQUENCY_DIV32); 
      
      /* Display the "BK0032" message on the LCD GLASS */
      BSP_LCD_GLASS_DisplayString((uint8_t*)"BK0032");
      
      /* Increment the Blink frequency */
      BlinkCounter++;
      break ;
      
    case  2:
      /* Configure all LCD segments and Coms to blink at frequency LCDCLK/64 */ 
      __HAL_LCD_BLINK_CONFIG(&LCDHandle, LCD_BLINKMODE_ALLSEG_ALLCOM, LCD_BLINKFREQUENCY_DIV64);
      
      /* Display the "BK0064" message on the LCD GLASS */
      BSP_LCD_GLASS_DisplayString((uint8_t*)"BK0064");
      
      /* Increment the Blink frequency */
      BlinkCounter++;
      break ;
      
    case  3:
      /* Configure all LCD segments and Coms to blink at frequency LCDCLK/128 */ 
      __HAL_LCD_BLINK_CONFIG(&LCDHandle, LCD_BLINKMODE_ALLSEG_ALLCOM, LCD_BLINKFREQUENCY_DIV128);
      
      /* Display the "BK0128" message on the LCD GLASS */
      BSP_LCD_GLASS_DisplayString((uint8_t*)"BK0128");
      
      /* Increment the blink frequency */
      BlinkCounter++;
      break ;
      
    case  4:
      /* Configure all LCD segments and Coms to blink at frequency LCDCLK/256 */ 
      __HAL_LCD_BLINK_CONFIG(&LCDHandle, LCD_BLINKMODE_ALLSEG_ALLCOM, LCD_BLINKFREQUENCY_DIV256);
      
      /* Display the "BK0256" message on the LCD GLASS */
      BSP_LCD_GLASS_DisplayString((uint8_t*)"BK0256");
      
      /* Increment the Blink frequency */
      BlinkCounter++;
      break ;
      
    case 5:
      /* Configure all LCD segments and Coms to blink at frequency LCDCLK/512 */ 
      __HAL_LCD_BLINK_CONFIG(&LCDHandle, LCD_BLINKMODE_ALLSEG_ALLCOM, LCD_BLINKFREQUENCY_DIV512);
      
      /* Display the "BK0512" message on the LCD GLASS */
      BSP_LCD_GLASS_DisplayString((uint8_t*)"BK0512");
      
      /* Increment the Blink frequency */
      BlinkCounter++;
      break;
    default:
      /* Configure all LCD segments and Coms to blink at frequency LCDCLK/1024 */ 
      __HAL_LCD_BLINK_CONFIG(&LCDHandle, LCD_BLINKMODE_ALLSEG_ALLCOM, LCD_BLINKFREQUENCY_DIV1024);
      
      /* Display the "BK1024" message on the LCD GLASS */
      BSP_LCD_GLASS_DisplayString((uint8_t*)"BK1024");
      BlinkCounter = 0;
      break ;
    }
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
