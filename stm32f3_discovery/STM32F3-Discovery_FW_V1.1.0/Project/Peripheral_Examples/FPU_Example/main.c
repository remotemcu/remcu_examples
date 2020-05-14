/**
  ******************************************************************************
  * @file    FPU_Example/main.c 
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    20-September-2012
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup STM32F3_Discovery_Peripheral_Examples
  * @{
  */

/** @addtogroup FPU_Example
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO float Result;
float Data0, Data1=1.0f, Data2=2.0f ;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief   Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /*!< At this stage the microcontroller clock setting is already configured, 
  this is done through SystemInit() function which is called from startup
  file (startup_stm32f30x.s) before to branch to application main.
  To reconfigure the default setting of SystemInit() function, refer to
  system_stm32f30x.c file
  */     
  
  /* Initialize Leds mounted on STM32F3-discovery */
  STM_EVAL_LEDInit(LED4);

  /* all operations are repeated in infinite loop, at the beginning of operation 
     the LED4 will be toggled, the time spend between two toggling operations 
     is the time of mathematical operation processing : this time is different  
     if FPU is enabled or not  */
  while(1)
  {
     /* Toggle LD4 */
    STM_EVAL_LEDToggle(LED4);
    
    /* Initialze float variables */
    Data0 = 1.0 ;
    Data1 = 2.0 ;	
    
    /* Start a set of mathematical operations */ 
    Result = fabsf(Data0) ;
  
    Result = -Data0 ;
  
    Result = Data1 + Data2 ;
  
    Result = Data1 - Data2 ;
  
    Result = Data0 + Data1 * Data2 ;
    Result = Data0 - Data1 * Data2 ;	
    Result = -Data0 + Data1 * Data2 ;
    Result = -Data0 - Data1 * Data2 ;	
  
    Result = fmaf(Data1,Data2,Data0) ;
    Result = fmaf(-Data1,Data2,Data0) ;	
    Result = fmaf(Data1,Data2,-Data0) ;
    Result = fmaf(-Data1,Data2,-Data0) ;
  
    Result = Data1 / Data2 ;
    Result = sqrtf(Data1) ;	
    Result = sqrt(Data1) ;		
    Result = fabs(Data1) ;
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
void assert_failed(uint8_t* file, uint32_t line)
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
