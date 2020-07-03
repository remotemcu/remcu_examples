/**
  ******************************************************************************
  * @file    FIREWALL/FIREWALL_VolatileData_Shared/Src/protected_volatile_data.c
  * @author  MCD Application Team
  * @brief   Definition of array to be located in Firewall protected volatile data segment (in SRAM)
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

#if defined(__ICCARM__)
#pragma section =".firewall_vdata"
#endif
/* Protected non-executable volatile data area (VDE = 0) */

/* The Firewall protects the area from 0x20000000 to  0x2000023F

  fw_init.VDataSegmentStartAddress    = 0x20000000;
  fw_init.VDataSegmentLength          = 0x240;   

  The scatter file locates protected_volatile_array in a sub-region starting at 0x20000200. 
    ...
    define symbol __ICFEDIT_protected_region_SRAM_b_start__    = 0x20000200;
    define symbol __ICFEDIT_protected_region_SRAM_b_end__      = 0x2000023F;
    ...
    define region protected_SRAM_b_region    = mem:[from __ICFEDIT_protected_region_SRAM_b_start__   to __ICFEDIT_protected_region_SRAM_b_end__];
    ...
    place in protected_SRAM_b_region   {readwrite section .firewall_vdata };
    ...
        
 */
#if defined(__ICCARM__)
#pragma default_variable_attributes = @ ".firewall_vdata"
#endif 
#if defined(__GNUC__)
uint32_t __attribute__((section(".vdata_protected_data")))  protected_volatile_array[N] = {50, 51, 52, 53, 54, 55, 56, 57, 58, 59};
#else
uint32_t protected_volatile_array[N] = {50, 51, 52, 53, 54, 55, 56, 57, 58, 59};
#endif 

/* Stop placing data in section protected_SRAM_b_region */
#if defined(__ICCARM__)
#pragma default_variable_attributes =
#endif





/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
