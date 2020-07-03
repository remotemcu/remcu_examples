/**
******************************************************************************
* @file    stm32l0xx_dualbankboot.h 
* @author  MCD Application Team
* @brief   Dual bank selection patch
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32L0XX_DUALBANKBOOT_H
#define __STM32L0XX_DUALBANKBOOT_H

#ifdef __cplusplus
extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_hal.h"
  
/* Exported types ------------------------------------------------------------*/
typedef enum 
{
  RAM_FUNCTION_StatusTypeDef_OK       = 0x00,
  RAM_FUNCTION_StatusTypeDef_ERROR    = 0x01,
  RAM_FUNCTION_StatusTypeDef_BUSY     = 0x02,
  RAM_FUNCTION_StatusTypeDef_TIMEOUT  = 0x03
} RAM_FUNCTION_StatusTypeDef;

/* Exported constants --------------------------------------------------------*/
/* User Defines: the values of these defined may be modified by user */
#define DATA_VALUE_SELECT           0x08080000 /* Location where to store the value allowing to boot on Bank2 or Bank1 */ 

#define DATA_VALUE_ADD_BANK1      DATA_VALUE_SELECT /* Location where to store the value allowing to boot on Bank2 */
#define DATA_VALUE_ADD_BANK2      0x08080C00        /* Location where to store the value allowing to boot on Bank1 */ 
  
#define DATA_BANK2_SELECTED         0x37       /* This value indicates that boot from Bank2 is required */
#define DATA_BANK1_SELECTED         0x73       /* This value indicates that boot from Bank1 is required */


/* Non user defines: these values must not be modified by user */
#define BANK2_START_ADDRESS_192KB  0x08018000  /* Bank 2 Start Address (192KB) */
#define BANK2_START_ADDRESS_128KB  0x08010000  /* Bank 2 Start Address (128KB) */
#define FLASH_START_ADDRESS        0x08000000  /* Flash Start Address (Bank1) */

#define ENGI_SALESTYPE_ADDRESS     0x1FF80034
#define ENGI_SALESTYPE_MASK        0x00000C00
#define ENGI_SALESTYPE_192KB       0x00000000
#define ENGI_SALESTYPE_128K        0x00000400
#define ENGI_SALESTYPE_64K         0x00000C00

#define SALESTYPE_192KB_DB         0x11001100
#define SALESTYPE_128K_DB          0x22002200
#define SALESTYPE_64K_SB           0x33003300

#define SYSCFG_MEMRMP_ADDRESS      0x40010000
#define SYSCFG_MEMMODE_MASK        0x00000003 
#define SYSCFG_MEMMODE_SYSMEM      0x00000001
#define SYSCFG_FBMODE_MASK         0x00000008
  
/* Exported macro ------------------------------------------------------------*/
  /** 
  * @brief  __RAM_FUNC definition
  */ 
#if defined ( __CC_ARM   )
  /* ARM Compiler
  ------------
  RAM functions are defined using the toolchain options. 
  Functions that are executed in RAM should reside in a separate source module.
  Using the 'Options for File' dialog you can simply change the 'Code / Const' 
  area of a module to a memory space in physical RAM.
  Available memory areas are declared in the 'Target' tab of the 'Options for Target'
  dialog. 
  */
  #define __RAM_FUNCTION RAM_FUNCTION_StatusTypeDef 
  
#elif defined ( __ICCARM__ )
  /* ICCARM Compiler
  ---------------
  RAM functions are defined using a specific toolchain keyword "__ramfunc". 
  */
  #define __RAM_FUNCTION __ramfunc RAM_FUNCTION_StatusTypeDef
  
#elif defined   (  __GNUC__  )
  /* GNU Compiler
  ------------
  RAM functions are defined using a specific toolchain attribute 
  "__attribute__((section(".RamFunc")))".
  */
  #define __RAM_FUNCTION RAM_FUNCTION_StatusTypeDef  __attribute__((section(".RamFunc")))
  
#endif  /* __CC_ARM */

/* Exported functions ------------------------------------------------------- */

/* This function has to be called by user application at startup */
__RAM_FUNCTION DualBankBoot(void);

/* This callback has to be implemented by user applciation, 
   It should reset value of DATA_VALUE_SELECT to zero using Flash driver functions. */
__RAM_FUNCTION DualBank_ResetFlag_Cbk(void); 
  
  
#ifdef __cplusplus
}
#endif 

#endif /*  __STM32L0XX_DUALBANKBOOT_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
