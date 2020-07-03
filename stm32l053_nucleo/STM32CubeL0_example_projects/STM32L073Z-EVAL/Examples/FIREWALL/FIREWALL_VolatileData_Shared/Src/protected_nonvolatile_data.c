/**
  ******************************************************************************
  * @file    FIREWALL/FIREWALL_VolatileData_Shared/Src/protected_nonvolatile_data.c
  * @author  MCD Application Team
  * @brief   Definition of array to be located in Firewall protected non-volatile data segment (in FLASH)
  *******************************************************************************    
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2016 STMicroelectronics</center></h2>
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

/* Protected non-volatile data area */

/* The Firewall protects the area from 0x08005000 to 0x080050FF

  fw_init.NonVDataSegmentStartAddress = 0x08005000;
  fw_init.NonVDataSegmentLength       = 0x100;

  The scatter file locates protected_volatile_array in the region starting at 0x08005000. 
    ...
    define symbol __ICFEDIT_protected_region_FLASH_start__    = 0x08005000;
    define symbol __ICFEDIT_protected_region_FLASH_end__      = 0x080050FF;
    ...
   define region protected_FLASH_region    = mem:[from __ICFEDIT_protected_region_FLASH_start__   to __ICFEDIT_protected_region_FLASH_end__];
    ...
   place in  protected_FLASH_region {readonly object protected_nonvolatile_data.o};
    ...
 */
#if defined(__GNUC__)
const uint32_t __attribute__((section(".nvdata_protected_data")))  protected_non_volatile_array[N] = {90, 91, 92, 93, 94, 95, 96, 97, 98, 99};
#else
const uint32_t protected_non_volatile_array[N] = {90, 91, 92, 93, 94, 95, 96, 97, 98, 99};
#endif
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
