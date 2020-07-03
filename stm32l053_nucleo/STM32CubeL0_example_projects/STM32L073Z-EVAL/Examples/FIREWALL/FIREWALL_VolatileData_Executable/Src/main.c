/**
  ******************************************************************************
  * @file    FIREWALL/FIREWALL_VolatileData_Executable/Src/main.c
  * @author  MCD Application Team
  * @brief   This sample code shows how to use STM32L0xx FIREWALL HAL API 
  *          to  protect an executable volatile data segment. 
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

/** @addtogroup FIREWALL_VolatileData_Executable
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Firewall configuration structure */
FIREWALL_InitTypeDef fw_init;  
 
/* Non-protected data, will be processed by protected function */ 
uint32_t unprotected_vdata[N] = {1, 4, 9, 16, 25, 36, 49, 64, 81, 100};

/* Variables used for code processing checking purposes */  
uint32_t expected_vdata_function_output_1 = 861;
uint32_t expected_vdata_function_output_2 = 1113;

/* Array located in protected volatile data segment (SRAM) */  
extern uint32_t protected_volatile_array[N];

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  uint32_t (*fptr)(uint32_t * array); /* Function pointer used to invoke executable protected volatile data segment */
  fptr = (uint32_t (*)(uint32_t * array))0x20002105; 
                                      /* Point at the proper address to obey call-gate entry procedure: 
                                         the address is the executable volatile data segment start + 1,
                                         where the executable volatile data segment start is the start address
                                         of the Firewall protected area + 4. */                                         
  uint32_t vdata_function_output = 0; /* output of the protected function */                                         
 
  
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

  /* Configure the system clock to 2 MHz */
  SystemClock_Config();

  /* Initialize LED1 */
  BSP_LED_Init(LED1);

  /* Enable SYSCFG clock to be able to enable the Firewall */
  __HAL_RCC_SYSCFG_CLK_ENABLE();    
     
  /* Enable Power Clock to be able to use RTC back-up registers.
     The latter are used to keep track of the example code proper unrolling.
     They are not related to Firewall functionality. */
  __HAL_RCC_PWR_CLK_ENABLE(); 
  HAL_PWR_EnableBkUpAccess();     
     
     
    /**************************************************************************/
    /*                                                                        */
    /*         RTC back-up initialization and RCC reset flags clear up        */    
    /*                                                                        */  
    /**************************************************************************/     
     
    /* Very first main() run if at this point RTC->BKP4R = 0 */
    if (READ_REG(RTC->BKP4R) == 0)
    {
       /* Mark that this is the first main() run */
       WRITE_REG( RTC->BKP4R, 0x1 );
       /* Initialize RTC back-up registers, used to log code progress
          or unexpected software resets / uncorrect processing
          (can be used for debugging purposes)   */
       WRITE_REG( RTC->BKP0R, 0x0 ); 
       WRITE_REG( RTC->BKP1R, 0x0 );
                
       /* Clear RCC reset indication flags to be
          able to detect a Firewall reset */
       __HAL_RCC_CLEAR_RESET_FLAGS();
    }
    else
    {
      /* This is not the first main() run, a software
         reset has occurred */
       
      if (__HAL_RCC_GET_FLAG(RCC_FLAG_FWRST) != RESET)
      {
        /* An unexpected Firewall reset has occurred, log it
          in filling up RTC back-up register BKP0R with 0x1 */      
        WRITE_REG( RTC->BKP0R, 0x1 );
      }
      else
      {
        /* No unexpected Firewall reset has occurred, log it
          in filling up RTC back-up register BKP1R with 0x1 */
        WRITE_REG( RTC->BKP1R, 0x1 );     
      }
      /* example unsuccessful end, exit in Error */ 
      Error_Handler();
    }
    
    
    
    /**************************************************************************/
    /*                                                                        */
    /*             Firewall parameters setting and enabling                   */
    /*                                                                        */  
    /**************************************************************************/
    
    /* No protected code segment (length set to 0) */   
    fw_init.CodeSegmentStartAddress      = 0x0;
    fw_init.CodeSegmentLength            = 0;
    
    /* No protected non-volatile data segment (length set to 0) */   
    fw_init.NonVDataSegmentStartAddress = 0x0;
    fw_init.NonVDataSegmentLength       = 0;
    
    /* Protected volatile data segment (in SRAM memory) start address and length */   
    fw_init.VDataSegmentStartAddress    = 0x20002100;
    fw_init.VDataSegmentLength          = 3840; /* 0xF00 bytes */  
    
    /* The protected volatile data segment can be executed */  
    fw_init.VolatileDataExecution       = FIREWALL_VOLATILEDATA_EXECUTABLE;
    
    /* The protected volatile data segment is not shared with non-protected
      application code */    
    fw_init.VolatileDataShared          = FIREWALL_VOLATILEDATA_NOT_SHARED;
    
    /* Firewall configuration */
    if (HAL_FIREWALL_Config(&fw_init) != HAL_OK)
    {
      Error_Handler();
    }  
    
    /* Enable Firewall */
    HAL_FIREWALL_EnableFirewall();
    
    /* From this point, the Firewall is closed */ 
    
    
    /**************************************************************************/
    /*                                                                        */
    /*              First access to protected executable data                 */
    /*                                                                        */  
    /**************************************************************************/  
     
    /* Invoke vdata_protected_function() API located at the proper address  to 
       follow the call gate sequence. 
       vdata_protected_function() is located in the protected volatile data 
       segment set as executable.
       This call will open the Firewall.
       Note that FPA bit is set to 0 at this point. */   
       
       
    /* In the example, the protected function processes
       non-protected input data and outputs an internal computation result. 
       For demonstration purposes, vdata_protected_function() accesses as well
       data located in the protected volatile data segment. 
       vdata_protected_function() itself ensures that FPA bit is set
       before exiting the protected area.
       */ 
     
    vdata_function_output = vdata_protected_function(unprotected_vdata); 
    
    /* The Firewall is now closed again. */
        
    /* Check that the output is that expected */
    if (vdata_function_output != expected_vdata_function_output_1)
    {
      /* Processing issue, log it in filling up RTC back-up register BKP0R 
         with 0x2, then exit in Error */  
      WRITE_REG( RTC->BKP0R, 0x2 ); 
      Error_Handler();
    }

    
    /**************************************************************************/
    /*                                                                        */
    /*                      Safety precaution, reset FPA                      */
    /*                                                                        */  
    /**************************************************************************/      
    
    /* Reset FPA bit to avoid any undesired attempt to access the protected  
       area (software will reset when jumping back to non-protected code) */
    HAL_FIREWALL_DisablePreArmFlag();
        
        
    /**************************************************************************/
    /*                                                                        */
    /*              Second access to protected executable data                */
    /*                                                                        */  
    /**************************************************************************/          
     
    /* For demonstration purposes, invoke protected executable data segment
      in resorting this time to a function pointer.
      Is exactly the same as invoking vdata_protected_function() but
      may be useful in some protected application execution cases.  */

    vdata_function_output = fptr(unprotected_vdata);
      
    /* The Firewall is now closed again. */      

    /* Check that the output is that expected */
    if (vdata_function_output != expected_vdata_function_output_2)
    {
      /* Processing issue, log it in filling up RTC back-up register BKP1R 
         with 0x2, then exit in Error */     
      WRITE_REG( RTC->BKP1R, 0x2 ); 
      Error_Handler();
    }    
    
    
    /**************************************************************************/
    /*                                                                        */
    /*                       Successful example execution                     */
    /*                                                                        */  
    /**************************************************************************/            

    /* Reset BKP4R to ensure a restart from a clean slate at the software next relaunch. */   
    WRITE_REG( RTC->BKP4R, 0x0 );
    
    /* Turn on LED1 */        
    BSP_LED_On(LED1);
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
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  
  /* Enable MSI Oscillator */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_5;
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
  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();
  
  /* The voltage scaling allows optimizing the power consumption when the device is 
     clocked below the maximum system frequency, to update the voltage scaling value 
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);
  
}



/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  /* Re-initialize BKP4R to ensure a restart
    from a clean slate at the software next relaunch. */   
  WRITE_REG( RTC->BKP4R, 0x0 );
  while(1)
  {
    /* In case of error, LED1 transmits a sequence of three dots, three dashes, three dots */
    BSP_LED_On(LED1); 
    HAL_Delay(300);
    BSP_LED_Off(LED1);
    HAL_Delay(300); 
    BSP_LED_On(LED1); 
    HAL_Delay(300);
    BSP_LED_Off(LED1);
    HAL_Delay(300);  
    BSP_LED_On(LED1); 
    HAL_Delay(300);
    BSP_LED_Off(LED1);
    HAL_Delay(300);   
    BSP_LED_On(LED1); 
    HAL_Delay(700);
    BSP_LED_Off(LED1);
    HAL_Delay(700); 
    BSP_LED_On(LED1); 
    HAL_Delay(700);
    BSP_LED_Off(LED1);
    HAL_Delay(700);  
    BSP_LED_On(LED1); 
    HAL_Delay(700);
    BSP_LED_Off(LED1);
    HAL_Delay(700); 
    BSP_LED_On(LED1); 
    HAL_Delay(300);
    BSP_LED_Off(LED1);
    HAL_Delay(300); 
    BSP_LED_On(LED1); 
    HAL_Delay(300);
    BSP_LED_Off(LED1);
    HAL_Delay(300);  
    BSP_LED_On(LED1); 
    HAL_Delay(300);
    BSP_LED_Off(LED1);
    HAL_Delay(800); 
  }
}


/**
  * @brief SYSTICK callback
  * @param None
  * @retval None
  */
void HAL_SYSTICK_Callback(void)
{
  HAL_IncTick();  
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
