/**
  ******************************************************************************
  * @file    k_module.c
  * @author  MCD Application Team   
  * @brief   This file provides the kernel module functions 
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

#define KMODULE_C
/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "main.h"
#include "k_config.h"
#include "k_module.h"

/** @addtogroup CORE
  * @{
  */

/** @defgroup KERNEL_MODULE
  * @brief Kernel module routines
  * @{
  */

/* External variables --------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static K_ModuleItem_Typedef    kmodule_info[MODULE_MAX];
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Add module inside the module list 
  * @param  kModuleId:   module id
            kModuleInfo: module information 
  * @retval none
  */
void kModule_Add(uint8_t kModuleId, K_ModuleItem_Typedef kModuleInfo)
{
  kmodule_info[kModuleId] = kModuleInfo;
}

/**
  * @brief  Check if the ressouce associated to a module are present 
  * @param  kModuleId:   module id
            kModuleInfo: module information 
  * @retval none
  */
KMODULE_RETURN kModule_CheckRessource(void)
{
uint8_t index;

  for(index = 0; index <  MODULE_MAX; index++)
  {
    if(kmodule_info[index].kModuleRessouceCheck != NULL)
    {
      if(kmodule_info[index].kModuleRessouceCheck() != KMODULE_OK)
      {
        return KMODULE_RESMISSIG;
      }
    }
  }
  return KMODULE_OK;
}

/**
  * @brief  Execute a module
  * @param  moduleid : id of the module
  * @retval Execution status 
  */
KMODULE_RETURN kModule_Execute(uint8_t moduleid) 
{
  
  /* Module Preprocessing  */
  if(kmodule_info[moduleid].kModulePreExec != NULL)
  {
    if(kmodule_info[moduleid].kModulePreExec() != KMODULE_OK)
    {
      return KMODULE_ERROR_PRE;
    }
  }
  
  /* Module Execution      */
  if(kmodule_info[moduleid].kModuleExec != NULL)
  {
    if(kmodule_info[moduleid].kModuleExec() != KMODULE_OK)
    {
      return KMODULE_ERROR_EXEC;
    }
  }
  
  /* Module Postprocessing */
  if(kmodule_info[moduleid].kModulePostExec != NULL)
  {
    if(kmodule_info[moduleid].kModulePostExec() != KMODULE_OK)
    {
      return KMODULE_ERROR_POST;
    }
  }
  
  return KMODULE_OK;
}
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
