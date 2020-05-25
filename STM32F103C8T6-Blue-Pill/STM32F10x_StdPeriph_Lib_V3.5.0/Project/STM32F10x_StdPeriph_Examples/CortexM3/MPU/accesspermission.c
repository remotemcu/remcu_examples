/**
  ******************************************************************************
  * @file    CortexM3/MPU/accesspermission.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Access rights configuration using Cortex-M3 MPU regions.
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

/** @addtogroup STM32F10x_StdPeriph_Examples
  * @{
  */

/** @addtogroup CortexM3_MPU
  * @{
  */
  
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define ARRAY_ADDRESS_START    (0x20002000UL)
#define ARRAY_SIZE             (0x09UL << 0UL)
#define ARRAY_REGION_NUMBER    (0x03UL << MPU_RNR_REGION_Pos) 

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

#if defined ( __CC_ARM   )
uint8_t privilegedreadonlyarray[32] __attribute__((at(0x20002000)));

#elif defined ( __ICCARM__ )
#pragma location=0x20002000
__no_init uint8_t privilegedreadonlyarray[32];

#elif defined   (  __GNUC__  )
uint8_t privilegedreadonlyarray[32] __attribute__((section(".ROarraySection")));

#elif defined   (  __TASKING__  )
uint8_t privilegedreadonlyarray[32] __at(0x20002000);
#endif

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  This function configure the access right using Cortex-M3 MPU regions.
  * @param  None
  * @retval None
  */
void accesspermission(void)
{
 uint8_t a; 
  
  /* Configure region for privilegedreadonlyarray as REGION NÝ3, 32byte and R 
     only in privileged mode */
  MPU->RNR  = ARRAY_REGION_NUMBER;
  MPU->RBAR |= ARRAY_ADDRESS_START;
  MPU->RASR |= ARRAY_SIZE | portMPU_REGION_PRIVILEGED_READ_ONLY;
  
  /* Read from privilegedreadonlyarray. This will not generate error */
  a = privilegedreadonlyarray[0];
  
  /* Uncomment the following line to write to privilegedreadonlyarray. This will
     generate error */
  //privilegedreadonlyarray[0] = 'e';
  
}

/**
  * @}
  */

/**
  * @}
  */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
