/**
  ******************************************************************************
  * @file    FIREWALL/FIREWALL_VolatileData_Shared/Src/main.c
  * @author  MCD Application Team
  * @brief   This sample code shows how to use STM32L0xx FIREWALL HAL API 
  *          to  protect a code segment, a non-volatile segment and a volatile 
  *          data segment. 
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

/** @addtogroup FIREWALL_VolatileData_Shared
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Firewall configuration structure */
FIREWALL_InitTypeDef fw_init;  
 
/* Array used for checking purposes */          
uint32_t expected_vdata[N] = {142, 147, 152, 157, 162, 167, 172, 177, 182, 187};

/* array located in protected non-volatile data segment (FLASH) */                                          
extern uint32_t protected_non_volatile_array[N];
/* array located in protected volatile data segment (SRAM) */  
extern uint32_t protected_volatile_array[N];

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void data_cmp(uint32_t *Text, uint32_t *RefText, uint32_t Size);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  uint32_t j = 0;                    /* counter variable */
  uint32_t check_vdata[N] = {0};     /* storing array for checking purposes */
 
  
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
    /*         RTC back-up initialization and RCC reset flags clear up,       */    
    /*                      Occurrence check for a                            */
    /*                Firewall-triggered software reset                       */
    /*                                                                        */  
    /**************************************************************************/     
     
    /* Very first main() run if at this point RTC->BKP4R = 0 */
    if (READ_REG(RTC->BKP4R) == 0)
    {
       /* Mark that this is the first main() run */
       WRITE_REG( RTC->BKP4R, 0x1 );
       /* Initialize RTC back-up registers, used to
          mark code progress or unexpected Firewall resets   */
       WRITE_REG( RTC->BKP0R, 0x0 ); 
       WRITE_REG( RTC->BKP1R, 0x0 );
       WRITE_REG( RTC->BKP2R, 0x0 );
       WRITE_REG( RTC->BKP3R, 0x0 );                
                
       /* Clear RCC reset indication flags to be
          able to detect a Firewall reset */
       __HAL_RCC_CLEAR_RESET_FLAGS();
    }
    else
    {
      /* This is not the first main() run, a software
         reset has occurred */
       
      /* Check if a Firewall reset has occurred and if this
         is indeed the case, and at which point in the code */
      if ((__HAL_RCC_GET_FLAG(RCC_FLAG_FWRST) != RESET)
          &&  ((READ_REG(RTC->BKP0R) == 2)))
      {
        /* A Firewall reset occurred when no more shared volatile 
           data were accessed while the Firewall was closed. 
           The reset was expected: example successful end */ 
                   
        /* Reset BKP4R to ensure a restart from a clean slate 
           at the software next relaunch. */   
        WRITE_REG( RTC->BKP4R, 0x0 );
         
        /* Turn on LED1 */
        BSP_LED_On(LED1);
        while (1)
        {
        }
      }
      else if ((__HAL_RCC_GET_FLAG(RCC_FLAG_FWRST) != RESET)
          &&  ((READ_REG(RTC->BKP0R) == 1)))
      {
        /* A Firewall reset occurred when non-volatile data were 
           accessed while the Firewall was closed. 
           The reset was expected, the test moves on */
           
        /* Mark that this is the second main() run */   
        WRITE_REG( RTC->BKP4R, 0x2 );
        /* Reset RTC back-up register BKP0R */              
        WRITE_REG( RTC->BKP0R, 0x0 ); 
        /* Clear RCC reset indication flags */
        __HAL_RCC_CLEAR_RESET_FLAGS();     
      }
      else
      {
        /* Example unsuccessful end; for debugging purposes, mark
           code incorrect end in filling up RTC back-up register BKP1R */
        WRITE_REG( RTC->BKP1R, 0x1 );     
        Error_Handler();
      }
    }           
    
    
    
    /**************************************************************************/
    /*                                                                        */
    /*             Firewall parameters setting and enabling                   */
    /*                                                                        */  
    /**************************************************************************/
    
    /* Protected code segment start address and length */   
    fw_init.CodeSegmentStartAddress      = 0x08004000;
    fw_init.CodeSegmentLength            = 512; /* 0x200 bytes */
    
    /* Protected non-volatile data segment (in FLASH memory) start address and length */   
    fw_init.NonVDataSegmentStartAddress = 0x08005000;
    fw_init.NonVDataSegmentLength       = 256;  /* 0x100 bytes */
    
    /* Protected volatile data segment (in SRAM memory) start address and length */   
    fw_init.VDataSegmentStartAddress    = 0x20000000;
    fw_init.VDataSegmentLength          = 576; /* 0x240 bytes */  
    
    /* The protected volatile data segment can't be executed */  
    fw_init.VolatileDataExecution       = FIREWALL_VOLATILEDATA_NOT_EXECUTABLE;
    
    /* The protected volatile data segment is shared with non-protected
      application code */    
    fw_init.VolatileDataShared          = FIREWALL_VOLATILEDATA_SHARED;
    
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
    /*                      Access to protected code                          */
    /*                                                                        */  
    /**************************************************************************/  
     
    /* Invoke protected_function() API located in the protected code
       segment at the proper address to follow the call gate sequence.
       This call will open the Firewall. 
       
       Note: user may as well resort to a function pointer to access the
       protected code. 
       Declaration should be as follows:
       
       {
         void (*fptr)(void); 
         fptr = (void (*)(void))0x08004005;  // point at the proper address to obey call gate entry procedure 
         ...                                
       
       And call:
       
         fptr();
       
       */   
     
    protected_function();
    
    /* At this point, the protected code segment has been exited without any issue
       (FPA bit has been set by protected_function() ).
       The Firewall is now closed again. */
     
    
    /* in case of second main() run, mark the code progression in setting   
       RTC back-up register BKP0R to 2 */   
    if (READ_REG(RTC->BKP4R) == 2)
    {
      WRITE_REG( RTC->BKP0R, 0x2 ); 
    }    
    
    /**************************************************************************/
    /*                                                                        */
    /*        Access volatile data memory while Firewall is closed.           */
    /*     Case of first main() run: no reset is expected since VDS = 1       */
    /*      Case of second main() run: reset is expected since VDS = 0        */    
    /*                                                                        */  
    /**************************************************************************/        
       
    for (j=0; j<N; j++)
    {
      check_vdata[j] = protected_volatile_array[j];
    } 
    
    if (READ_REG(RTC->BKP4R) == 2)
    {
     /* If the code unrolls down to this point in the case of the second
        main() run, it means that an issue occurred as a Firewall reset was 
        expected.
        For debugging purposes, mark code incorrect end in filling up RTC  
        back-up register BKP3R */    
      WRITE_REG( RTC->BKP3R, 0x1 ); 
      Error_Handler();
    }
    
    /* No issue up to this point:
       check that protected_function() has updated protected_volatile_array[]
       as expected */
    data_cmp(check_vdata, expected_vdata, N);
    
    /* Mark that the protected code access as well as the 
      protected volatile data segment access were successful 
      in setting  RTC back-up register BKP0R to 1 */
    WRITE_REG( RTC->BKP0R, 0x1 ); 
    
   
    
    /**************************************************************************/
    /*                                                                        */
    /*                 Purposedly trigger a Firewall Reset                    */
    /*                                                                        */  
    /**************************************************************************/ 
     
    /* Access the non-volatile data segment while the Firewall is closed. 
       A software reset is expected. */
     check_vdata[0] = protected_non_volatile_array[0];  
                        
     /* If the code unrolls down to the line below, it means that an issue 
       occurred as a Firewall reset was expected.
       For debugging purposes, mark code incorrect end in filling up RTC  
       back-up register BKP2R */
      WRITE_REG( RTC->BKP2R, 0x1 ); 
      Error_Handler();
                          
      return  check_vdata[0];

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
    
    
/**
  * @brief  buffers comparison
  * @param  Text, first buffer
  * @param  RefText, second buffer
  * @param  Size, buffers size in bytes       
  * @retval None
  */
void data_cmp(uint32_t *Text, uint32_t *RefText, uint32_t Size) 
{
uint32_t i;
   
  for (i=0;i<Size;i++)
  {
    if (Text[i] != RefText[i])
    {
      Error_Handler();   
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
