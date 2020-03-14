/**
  ******************************************************************************
  * @file    CortexM3/MPU/main.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************  
  */ 

/* Includes ------------------------------------------------------------------*/

#include "main.h"
#include "stm32_eval.h"

/** @addtogroup STM32F10x_StdPeriph_Examples
  * @{
  */

/** @addtogroup CortexM3_MPU
  * @{
  */
  
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define ACCESS_PERMISSION

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void MPU_SETUP(void);

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
  
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f10x_xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f10x.c file
     */  
 
  /* Configure LED1 and LED2 */
  STM_EVAL_LEDInit(LED1);
  STM_EVAL_LEDInit(LED2);
  
  /* Set MPU regions */
  MPU_SETUP();

  #ifdef ACCESS_PERMISSION
    accesspermission();
  #endif  

  /* Infinite loop */
  while (1)
  {
    /* Turn On LED1 */
    STM_EVAL_LEDOn(LED1); 
  }
}

/**
  * @brief  Configures the main MPU regions.
  * @param  None
  * @retval None
  */
void MPU_SETUP(void)
{
  /* Disable MPU */
  MPU->CTRL &= ~MPU_CTRL_ENABLE_Msk;
  
  /* Configure RAM region as Region NÝ0, 8kB of size and R/W region */
  MPU->RNR  = RAM_REGION_NUMBER;
  MPU->RBAR = RAM_ADDRESS_START;
  MPU->RASR = RAM_SIZE | portMPU_REGION_READ_WRITE;
  
  /* Configure FLASH region as REGION NÝ1, 1MB of size and R/W region */
  MPU->RNR  = FLASH_REGION_NUMBER;
  MPU->RBAR = FLASH_ADDRESS_START;
  MPU->RASR = FLASH_SIZE | portMPU_REGION_READ_WRITE;
  
  /* Configure Peripheral region as REGION NÝ2, 0.5GB of size, R/W and Execute
  Never region */
  MPU->RNR  = PERIPH_REGION_NUMBER;  
  MPU->RBAR = PERIPH_ADDRESS_START;
  MPU->RASR = PERIPH_SIZE |portMPU_REGION_READ_WRITE | MPU_RASR_XN_Msk;
  
  /* Enable the memory fault exception */
  SCB->SHCSR |= SCB_SHCSR_MEMFAULTENA_Msk;
   
  /* Enable MPU */
  MPU->CTRL |= MPU_CTRL_PRIVDEFENA_Msk | MPU_CTRL_ENABLE_Msk;
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

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
