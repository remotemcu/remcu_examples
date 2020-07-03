/**
  ******************************************************************************
  * @file    IAP_Main/Src/flash_if.c 
  * @author  MCD Application Team
  * @brief   This file provides all the memory related operation functions.
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

/** @addtogroup STM32L0xx_IAP
  * @{
  */

/* Includes ------------------------------------------------------------------*/
#include "flash_if.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Unlocks Flash for write access
  * @param  None
  * @retval None
  */
void FLASH_If_Init(void)
{
  /* Unlock the Program memory */
  HAL_FLASH_Unlock();

  /* Clear all FLASH flags */
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_SIZERR |
                         FLASH_FLAG_OPTVERR | FLASH_FLAG_RDERR | FLASH_FLAG_FWWERR |
                         FLASH_FLAG_NOTZEROERR);
  /* Unlock the Program memory */
  HAL_FLASH_Lock();
}

/**
  * @brief  This function does an erase of all user flash area
  * @param  start: start of user flash area
  * @retval FLASHIF_OK : user flash area successfully erased
  *         FLASHIF_ERASEKO : error occurred
  */
uint32_t FLASH_If_Erase(uint32_t start)
{
  FLASH_EraseInitTypeDef desc;
  uint32_t result = FLASHIF_OK;

  HAL_FLASH_Unlock();

  desc.PageAddress = start;
  desc.TypeErase = FLASH_TYPEERASE_PAGES;

/* NOTE: Following implementation expects the IAP code address to be < Application address */  
  if (start < FLASH_START_BANK2 )
  {
    desc.NbPages = (FLASH_START_BANK2 - start) / FLASH_PAGE_SIZE;
    if (HAL_FLASHEx_Erase(&desc, &result) != HAL_OK)
    {
      result = FLASHIF_ERASEKO;
    }
  }

  if (result == FLASHIF_OK )
  {
    desc.PageAddress = ABS_RETURN(start, FLASH_START_BANK2);
    desc.NbPages = (USER_FLASH_END_ADDRESS - ABS_RETURN(start, FLASH_START_BANK2)) / FLASH_PAGE_SIZE;
    if (HAL_FLASHEx_Erase(&desc, &result) != HAL_OK)
    {
      result = FLASHIF_ERASEKO;
    }
  }
  
  HAL_FLASH_Lock();

  return result;
}	

/* Public functions ---------------------------------------------------------*/
/**
  * @brief  This function writes a data buffer in flash (data are 32-bit aligned).
  * @note   After writing data buffer, the flash content is checked.
  * @param  destination: start address for target location
  * @param  p_source: pointer on buffer with data to write
  * @param  length: length of data buffer (unit is 32-bit word)
  * @retval uint32_t 0: Data successfully written to Flash memory
  *         1: Error occurred while writing data in Flash memory
  *         2: Written Data in flash memory is different from expected one
  */
uint32_t FLASH_If_Write(uint32_t destination, uint32_t *p_source, uint32_t length)
{
  uint32_t status = FLASHIF_OK;
  uint32_t *p_actual = p_source; /* Temporary pointer to data that will be written in a half-page space */
  uint32_t i = 0;

  HAL_FLASH_Unlock();

  while (p_actual < (uint32_t*)(p_source + length))
  {    
    /* Write the buffer to the memory */
    if (HAL_FLASHEx_HalfPageProgram( destination, p_actual ) == HAL_OK) /* No error occurred while writing data in Flash memory */
    {
      /* Check if flash content matches memBuffer */
      for (i = 0; i < WORDS_IN_HALF_PAGE; i++)
      {
        if ((*(uint32_t*)(destination + 4 * i)) != p_actual[i])
        {
          /* flash content doesn't match memBuffer */
          status = FLASHIF_WRITINGCTRL_ERROR;
          break;
        }
      }

      /* Increment the memory pointers */
      destination += FLASH_HALF_PAGE_SIZE;
      p_actual += WORDS_IN_HALF_PAGE;
    }
	else
	{
		status = FLASHIF_WRITING_ERROR;
	}

    if ( status != FLASHIF_OK )
    {
      break;
    }
  }

  HAL_FLASH_Lock();

  return status;
}	

/**
  * @brief  Returns the write protection status of application flash area.
  * @param  None
  * @retval If a sector in application area is write-protected returned value is a combinaison
            of the possible values : FLASHIF_PROTECTION_WRPENABLED, FLASHIF_PROTECTION_PCROPENABLED, ...
  *         If no sector is write-protected FLASHIF_PROTECTION_NONE is returned.
  */
uint32_t FLASH_If_GetWriteProtectionStatus(void)
{
  FLASH_OBProgramInitTypeDef config;
  FLASH_AdvOBProgramInitTypeDef adv_config;
  uint32_t wrp1_status = 0, wrp2_status = 0, sectornumber = 0;
  uint32_t protected = FLASHIF_PROTECTION_NONE;

  /* Get the current configuration */
  HAL_FLASHEx_OBGetConfig( &config );
  HAL_FLASHEx_AdvOBGetConfig( &adv_config );

  sectornumber = FLASH_SECTOR_NUMBER;

  /* Not taking application size into account, all the memory starting the beginning address is checked */
  /* As the APPLICATION_ADDRESS is a define constant, this code may be omitted by the optimization in   */
  /* compiler. However it is present for case of user specified APPLICATION_ADDRESS value.               */
  if ( adv_config.BootConfig == OB_BOOT_BANK1 )  /* Test on the user application to be ran in Bank1 */
  {
    if (sectornumber < 32)
    {
      wrp1_status = config.WRPSector & FLASH_PROTECTED_SECTORS;
      wrp2_status = config.WRPSector2 & OB_WRP2_AllPages;
    }
    else
    {
      wrp1_status = config.WRPSector;
      wrp2_status = config.WRPSector2 & ~(uint32_t)(((1 << (sectornumber - 32 )) - 1));
    }
  }
  else /* running from bank 2 */
  {
    if (sectornumber < 32)
    {
      wrp1_status = config.WRPSector & ( FLASH_PROTECTED_SECTORS | (~FLASH_BANK1_MASK) );
      wrp2_status = config.WRPSector2 & OB_WRP2_AllPages;
    }
    else
    {
      wrp1_status = config.WRPSector & (~FLASH_BANK1_MASK);
      wrp2_status = config.WRPSector2 & ~(uint32_t)(((1 << (sectornumber - 32 )) - 1));
    }
  }

  /* Final evaluation of status */
  if ((wrp1_status != 0) || (wrp2_status != 0))
  {
    protected = FLASHIF_PROTECTION_WRPENABLED;
  }
  
  return protected;	
}

/**
  * @brief  Configure the write protection status of user flash area.
  * @param  protectionstate : FLASHIF_WRP_DISABLE or FLASHIF_WRP_ENABLE the protection
  * @retval uint32_t FLASHIF_OK if change is applied.
  */
uint32_t FLASH_If_WriteProtectionConfig(uint32_t protectionstate)
{
  FLASH_OBProgramInitTypeDef config_new, config_old;
  FLASH_AdvOBProgramInitTypeDef adv_config;
  HAL_StatusTypeDef result;
  uint32_t sectornumber = 0;

  sectornumber = FLASH_SECTOR_NUMBER;

  /* Get the current configuration */
  HAL_FLASHEx_OBGetConfig( &config_old );
  HAL_FLASHEx_AdvOBGetConfig( &adv_config );

  /* The parameter says whether we turn the protection on or off */
  config_new.WRPState = (protectionstate == FLASHIF_WRP_ENABLE ? OB_WRPSTATE_ENABLE : OB_WRPSTATE_DISABLE);

  /* We want to modify only the Write protection */
  config_new.OptionType = OPTIONBYTE_WRP;
  
  /* No read protection, keep BOR and reset settings */
  config_new.RDPLevel = OB_RDP_LEVEL_0;
  config_new.BORLevel = config_old.BORLevel;
  config_new.USERConfig = config_old.USERConfig;

  /* Not taking application size into account, all the memory starting the beginning address is checked */
  /* As the APPLICATION_ADDRESS is a define constant, this code may be omitted by the optimization in   */
  /* compiler. However it is present for case of user specified APPLICATION_ADDRESS value.               */
  /* Not taking application size into account, all the memory starting the beginning address is modified */
  if ( adv_config.BootConfig != OB_BOOT_BANK1 ) /* BANK2 active for boot */
  {
    if (sectornumber < 32)
    {
      config_new.WRPSector = FLASH_BANK1_MASK & (config_old.WRPSector | FLASH_PROTECTED_SECTORS);
      config_new.WRPSector2 = config_old.WRPSector2 | OB_WRP2_AllPages;
    }
    else
    {
      config_new.WRPSector = FLASH_BANK1_MASK;
      config_new.WRPSector2 = config_old.WRPSector2 | (~(uint32_t)((1 << (sectornumber - 32)) - 1));
    }
  }
  else /* Memory ordering normal */
  {
    if (sectornumber < 32)
    {
      config_new.WRPSector = config_old.WRPSector | FLASH_PROTECTED_SECTORS;
      config_new.WRPSector2 = config_old.WRPSector2 | OB_WRP2_AllPages;
    }
    else
    {
      config_new.WRPSector = 0;
      config_new.WRPSector2 = config_old.WRPSector2 | (~(uint32_t)((1 << (sectornumber - 32)) - 1));
    }
  }

  /* Initiating the modifications */
  result = HAL_FLASH_OB_Unlock();

  /* program if unlock is successful */
  if (result == HAL_OK)
  {
    HAL_FLASHEx_AdvOBProgram(&adv_config);
    result = HAL_FLASHEx_OBProgram(&config_new);
  }

  return (result == HAL_OK ? FLASHIF_OK: FLASHIF_PROTECTION_ERRROR);
}
/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
