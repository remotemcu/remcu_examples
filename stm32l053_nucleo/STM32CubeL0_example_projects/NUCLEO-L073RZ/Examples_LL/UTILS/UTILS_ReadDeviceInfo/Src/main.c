/**
  ******************************************************************************
  * @file    Examples_LL/UTILS/UTILS_ReadDeviceInfo/Src/main.c
  * @author  MCD Application Team
  * @brief   This example describes how to read UID, Device ID and Revision ID 
  *          through the STM32L0xx UTILS LL API.
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

/** @addtogroup STM32L0xx_LL_Examples
  * @{
  */

/** @addtogroup UTILS_ReadDeviceInfo
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Buffer used for displaying different UTILS info */
uint8_t aShowDeviceID[30]    = {0};
uint8_t aShowRevisionID[30]  = {0};
uint8_t aShowUIDWord0[32]    = {0};
uint8_t aShowUIDWord1[32]    = {0};
uint8_t aShowUIDWord2[32]    = {0};

/* Private function prototypes -----------------------------------------------*/
void     GetMCUInfo(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /* Get different information available in the MCU */
  GetMCUInfo();
  
  /* Infinite loop */
  while (1)
  {
  }
}

/**
  * @brief  Get different information available in the MCU (Device ID, Revision ID & UID)
  * @param  None
  * @retval None
  */
void GetMCUInfo(void)
{
  /* Display Device ID in string format */
  sprintf((char*)aShowDeviceID,"Device ID = 0x%lX", LL_DBGMCU_GetDeviceID());
  
  /* Display Revision ID in string format */
  sprintf((char*)aShowRevisionID,"Revision ID = 0x%lX", LL_DBGMCU_GetRevisionID());

  /* Display UID Word0 */
  sprintf((char*)aShowUIDWord0,"UID Word0 = 0x%lX", LL_GetUID_Word0());
  
  /* Display UID Word1 */
  sprintf((char*)aShowUIDWord1,"UID Word1 = 0x%lX", LL_GetUID_Word1());
  
  /* Display UID Word2 */
  sprintf((char*)aShowUIDWord2,"UID Word2 = 0x%lX", LL_GetUID_Word2());

}

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
