/**
  ******************************************************************************
  * @file    FIREWALL/FIREWALL_VolatileData_Shared/Src/protected_code.c
  * @author  MCD Application Team
  * @brief   Definition of functions to be located in Firewall protected code area (in FLASH)
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


extern uint32_t protected_non_volatile_array[N];
extern uint32_t protected_volatile_array[N];




/* function in protected segment, direct access to this
   function follows call gate sequence since *.icf is written
   to locate it at the proper address: protected code segment start + 4:
   
   Firewall is configured as follows:
   fw_init.CodeSegmentStartAddress      = 0x08004000;
   
   and  *icf file places protected_function places protected_function() at = 0x08004004:
      ...
      define symbol __ICFEDIT_protected_region_ROM_start__    = 0x08004004;
      define symbol __ICFEDIT_protected_region_ROM_end__      = 0x080041FF;    
      ...
      define region protected_ROM_region      = mem:[from __ICFEDIT_protected_region_ROM_start__   to __ICFEDIT_protected_region_ROM_end__];
      ...
      place in  protected_ROM_region {readonly object protected_code.o};
     */
	 
	 #if defined(__GNUC__)
void __attribute__((section(".nvdata_protected_code"))) protected_function(void)
#else
void protected_function(void)
#endif
{
  uint32_t i = 1;
  uint32_t j = 0;
                                   

  /* In case of protected_function() second call, during main() second run(),
     VDS bit is reset: protected volatile data are no more shared with 
     non-protected code */
  if (READ_REG(RTC->BKP4R) == 2)
  {
    __HAL_FIREWALL_VOLATILEDATA_SHARED_DISABLE();
  }


  /* Application processing filling up the array located
    in the protected volatile data segment */
   
   for (j=0; j<N; j++)
   {
     protected_volatile_array[j] += (2*i+3*j + protected_non_volatile_array[j]) ;
   }
   
   /* Set FPA bit for a proper closure of the Firewall when exiting the
      protected function */
   __HAL_FIREWALL_PREARM_ENABLE();
   
   return;

}






/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
