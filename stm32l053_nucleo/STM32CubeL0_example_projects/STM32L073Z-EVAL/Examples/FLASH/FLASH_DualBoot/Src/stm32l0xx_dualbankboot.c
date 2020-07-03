/**
******************************************************************************
* @file    stm32l0xx_dualbankboot.c 
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

/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_dualbankboot.h"

/* Private typedef -----------------------------------------------------------*/
typedef  void (*pFunction)(void);

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint32_t SalesType = 0, Bank2Addr = 0, CurrAddr = 0;
__IO uint32_t check_value = 0 ;
__IO uint32_t DelayIdx = 0;

pFunction Jump_To_Application;
uint32_t JumpAddress;

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : DualBankBootSelection
* Description    : Manages the boot from Bank1 or Bank2 depending on available code.
*                  The function uses a flag value stored in Data memory at address
*                  DATA_VALUE_SELECT (ie. 0x08080000) that can be customized by user
*                  to select if Boot shall be done from Bank1 or Bank2.
*                  User has to write this value into Data memory at specified address
*                  in order to trigger the dual boot mechanism in this function.
*                  This function may be called at startup of the user code. 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
__RAM_FUNCTION DualBankBoot(void)
{  
  /* Check what is the device salestype (192KB, 128KB or 64KB) */
  if  (((*(__IO uint32_t *) (ENGI_SALESTYPE_ADDRESS)) &  ENGI_SALESTYPE_MASK) == ENGI_SALESTYPE_192KB)
  {
    /* If 192KB salestype */
    SalesType = SALESTYPE_192KB_DB;
  }
  else if (( (*(__IO uint32_t *) (ENGI_SALESTYPE_ADDRESS)) &  ENGI_SALESTYPE_MASK) == ENGI_SALESTYPE_128K)
  {
    /* If 128KB salestype */
    SalesType = SALESTYPE_128K_DB;
  }
  else
  {
    /* If 64KB salestype */
    SalesType = SALESTYPE_64K_SB;
  }
  
  /* Enable the SYSCFG APB Clock */
  *(__IO uint32_t *) 0x40021034 |= ((uint32_t)0x00000001); 
  
  /************************ Get salestype  ***********************/
  
  /* Manage case when single bank salestypes are used */
  if (SalesType !=  SALESTYPE_64K_SB)
  {
    /* Get bank2 start address depending on the package */
    Bank2Addr = (SalesType ==  SALESTYPE_192KB_DB)? BANK2_START_ADDRESS_192KB : BANK2_START_ADDRESS_128KB;
    
    /************************ Check Data value  ************************/
    /* If data value selected to remap Bank2  */
    if (*(__IO uint8_t *)DATA_VALUE_SELECT == DATA_BANK2_SELECTED)
    { /* If already on Bank2 */
     if ( (*(__IO uint32_t *)SYSCFG_MEMRMP_ADDRESS) &= SYSCFG_FBMODE_MASK )
     {
       CurrAddr = 0;
     }
     else
     {
      /* Remap Bank2 at 0x0800 0000 */
      (*(__IO uint32_t *)SYSCFG_MEMRMP_ADDRESS) |= SYSCFG_FBMODE_MASK;
      CurrAddr = FLASH_START_ADDRESS; 
      
      /* Call function that will reset the DATA_VALUE_SELECT value to 0 to prevent infinitely looping into this function */
      DualBank_ResetFlag_Cbk();
     }
    }
    
    /* If data value selected to remap Bank1  */
    else if (*(__IO uint8_t *)DATA_VALUE_SELECT == DATA_BANK1_SELECTED)
    { /* If already on Bank1 */
      if ( ((*(__IO uint32_t *)SYSCFG_MEMRMP_ADDRESS) & SYSCFG_FBMODE_MASK) == 0x0 )
     {
       CurrAddr = 0;
     }
     else
     {
      /* remap Bank1 at 0x0800 0000 */
      (*(__IO uint32_t *)SYSCFG_MEMRMP_ADDRESS) &= (~SYSCFG_FBMODE_MASK);
      CurrAddr = FLASH_START_ADDRESS;
	        
      /* Call function that will reset the DATA_VALUE_SELECT value to 0 to prevent infinitely looping into this function */
      DualBank_ResetFlag_Cbk();
     }
    }
    
    else 
    {
      /* If data value seleted is different from both data banks   */
      CurrAddr = 0;
    }
    
    if (CurrAddr != 0)
    {
      /* Introduce a delay of few cycles to make sure that FB_MODE bit has been written  */
      DelayIdx = 100;
      while ((DelayIdx--) != 0)
      {}
      
      /* Jump to the application code in Valid Bank */
      /* Reinitialize the Stack pointer and jump to application address */
      JumpAddress = *(__IO uint32_t *) (CurrAddr + 4);
      Jump_To_Application = (pFunction) JumpAddress;
      /* Initialize user application's Stack Pointer */
      __set_MSP(*(__IO uint32_t*) CurrAddr);
      Jump_To_Application();
      
      /* This instruction is added to prevent compiler from generating a POP instruction
      before the jump which means the Stack Pointer altered */
      while (1)
      {}
    }
  }
  
  /* No Dual Bank is requested or possible */
  return RAM_FUNCTION_StatusTypeDef_OK;
}

/*******************************************************************************
* Function Name  : DualBank_ResetFlag_Cbk
* Description    : This function is called by DualBankBoot() function, its prototype
*                  is declared in stm32l0xx_dualbankboot.h and it should be implemented
*                  by user in his application code. 
*                  This function should write zero to the DATA_VALUE_SELECT (ie. 0x08080000).
*                  An implementation example is provide below just for guidance.
*                  
*  -- IMPORTANT --
*   Please make sure that this function and all functions called by this function 
*   are located in SRAM.
*   
*
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
 
__RAM_FUNCTION DualBank_ResetFlag_Cbk(void)
{
  
  if((FLASH->PECR & FLASH_PECR_PRGLOCK) != RESET)
  {
    /* Unlocking FLASH_PECR register access */
    if((FLASH->PECR & FLASH_PECR_PELOCK) != RESET)
     {  
         FLASH->PEKEYR = FLASH_PEKEY1;
         FLASH->PEKEYR = FLASH_PEKEY2;
     }
 
    /* Unlocking the program memory access */
    FLASH->PRGKEYR = FLASH_PRGKEY1;
    FLASH->PRGKEYR = FLASH_PRGKEY2;  
  }  
  
  /* Write data */
  *(__IO uint8_t *)DATA_VALUE_SELECT = 0x00;
  
  return RAM_FUNCTION_StatusTypeDef_OK;
    
}   


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
