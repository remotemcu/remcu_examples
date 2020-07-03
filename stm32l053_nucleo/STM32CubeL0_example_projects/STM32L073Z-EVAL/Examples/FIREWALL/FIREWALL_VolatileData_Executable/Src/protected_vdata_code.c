/**
  ******************************************************************************
  * @file    FIREWALL/FIREWALL_VolatileData_Executable/Src/protected_vdata_code.c 
  * @author  MCD Application Team
  * @brief   Definition of function located in Firewall protected volatile data area (in SRAM)
  ******************************************************************************
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

/* Data located in protected volatile data segment */
extern uint32_t protected_volatile_array[N];

/**
  * @brief  Protected function located in volatile data segment set as executable
  * @param array: input array of length N
  * @note  The API processes the input data in carrying out some computations
  *        resorting (for demonstration purposes) to data located in the
  *        protected data segment. 
  *        At the same time, the protected data are updated. 
  *          
  *        The Firewall protects the area from 0x20002100 to  0x20002FFF
  *        
  *          fw_init.VDataSegmentStartAddress    = 0x20002100;
  *          fw_init.VDataSegmentLength          = 0xF00;
  *               
  *        The scatter file locates vdata_protected_function in a sub-region starting at 0x20002104. 
  *          ...
  *          define symbol __ICFEDIT_protected_region_SRAM_a_start__    = 0x20002104;
  *          define symbol __ICFEDIT_protected_region_SRAM_a_end__      = 0x20002DFF;
  *          ...
  *          define region protected_SRAM_a_region    = mem:[from __ICFEDIT_protected_region_SRAM_a_start__   to __ICFEDIT_protected_region_SRAM_a_end__];
  *          ...
  *          place in  protected_SRAM_a_region {readonly object protected_vdata_code.o};
  *          ...
  *            
  * @retval uint32_t value, result of function internal processing
  */
#if defined(__GNUC__)
uint32_t __attribute__((section(".vdata_protected_code"))) vdata_protected_function(uint32_t * array)
#else
uint32_t vdata_protected_function(uint32_t * array)
#endif
{
  uint32_t i = 5;
  uint32_t j = 0;


    for (j=0; j<N; j++)
   {
     protected_volatile_array[j] += i+j*j + array[j];
   }
   
   i =   protected_volatile_array[N-1] + 11 * protected_volatile_array[0]; 
   

   /* Ensure no reset occurs when jumping to non-protected code */
   __HAL_FIREWALL_PREARM_ENABLE();
   
   return i;

}



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
