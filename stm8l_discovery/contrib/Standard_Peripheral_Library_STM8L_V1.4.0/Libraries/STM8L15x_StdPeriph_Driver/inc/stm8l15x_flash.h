/**
  ******************************************************************************
  * @file    stm8l15x_flash.h
  * @author  MCD Application Team
  * @version V1.4.0
  * @date    09/24/2010
  * @brief   This file contains all the functions prototypes for the FLASH firmware
  *          library.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTIOzzzN WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM8L15x_FLASH_H__
#define __STM8L15x_FLASH_H__

/* Includes ------------------------------------------------------------------*/
#include "stm8l15x.h"

/** @addtogroup STM8L15x_StdPeriph_Driver
  * @{
  */

/* Exported variables ------------------------------------------------------- */
/* Exported constants --------------------------------------------------------*/

/** @addtogroup FLASH_Exported_Constants
  * @{
  */
#define FLASH_PROGRAM_START_PHYSICAL_ADDRESS       ((uint32_t)0x00008000) /*!< Flash: start address */  
#define FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS   ((uint32_t)0x00001000) /*!< Data Eeprom: start address */
#ifdef STM8L15X_HD
 #define FLASH_PROGRAM_END_PHYSICAL_ADDRESS         ((uint32_t)0x00017FFF) /*!< Flash: end address */
 #define FLASH_DATA_EEPROM_END_PHYSICAL_ADDRESS     ((uint32_t)0x000017FF) /*!< Data Eeprom: end address */
 #define FLASH_PROGRAM_BLOCKS_NUMBER                ((uint16_t)0x200)      /*!< Flash memory: total number of Block */
 #define FLASH_DATA_EEPROM_BLOCKS_NUMBER            ((uint8_t)0x10)        /*!< Data EEprom: total number of Block */
#elif defined (STM8L15X_MD) || defined (STM8L15X_MDP)
 #define FLASH_PROGRAM_END_PHYSICAL_ADDRESS         ((uint32_t)0x0000FFFF) /*!< Flash: end address */
 #define FLASH_DATA_EEPROM_END_PHYSICAL_ADDRESS     ((uint32_t)0x000013FF) /*!< Data Eeprom: end address */
 #define FLASH_PROGRAM_BLOCKS_NUMBER                ((uint16_t)0x100)      /*!< Flash memory: total number of Block */
 #define FLASH_DATA_EEPROM_BLOCKS_NUMBER            ((uint8_t)0x8)         /*!< Data EEprom: total number of Block */
#endif /* STM8L15X_HD */
#define FLASH_OPTION_BYTES_START_PHYSICAL_ADDRESS  ((uint32_t)0x00004800) /*!< Option bytes: start address */
#define FLASH_OPTION_BYTES_END_PHYSICAL_ADDRESS    ((uint32_t)0x0000480A) /*!< Option bytes: end address */
#define FLASH_BLOCK_SIZE                           ((uint8_t)0x80)        /*!< Number of bytes in a Block 
                                                                              (common for Program and Data EEprom memories) */
#define FLASH_RASS_KEY1                            ((uint8_t)0x56)        /*!< First RASS key */
#define FLASH_RASS_KEY2                            ((uint8_t)0xAE)        /*!< Second RASS key */
#define FLASH_READOUTPROTECTION_KEY                ((uint8_t)0xAA)        /*!< Read out protection key */

  /**
    * @}
    */

  /* Exported types ------------------------------------------------------------*/

  /** @addtogroup FLASH_Exported_Types
    * @{
    */

  /**
    * @brief FLASH Memory types
    */
  typedef enum
  {
    FLASH_MemType_Program      = (uint8_t)0xFD, /*!< Program memory */
    FLASH_MemType_Data         = (uint8_t)0xF7  /*!< Data EEPROM memory */
  } FLASH_MemType_TypeDef;

/**
  * @brief FLASH programming modes
  */
typedef enum
{
  FLASH_ProgramMode_Standard = (uint8_t)0x00, /*!< Standard programming mode */
  FLASH_ProgramMode_Fast     = (uint8_t)0x10  /*!< Fast programming mode */
} FLASH_ProgramMode_TypeDef;

/**
  * @brief FLASH fixed programming time
  */
typedef enum
{
  FLASH_ProgramTime_Standard = (uint8_t)0x00, /*!< Standard programming time fixed at 1/2 tprog */
  FLASH_ProgramTime_TProg    = (uint8_t)0x01  /*!< Programming time fixed at tprog */
} FLASH_ProgramTime_TypeDef;

/**
  * @brief FLASH power mode
  */
typedef enum
{
  FLASH_Power_IDDQ = (uint8_t)0x00,  /*!< Flash program and data EEPROM in IDDQ */
  FLASH_Power_On    = (uint8_t)0x01  /*!< Flash program and data EEPROM not in IDDQ */
} FLASH_Power_TypeDef;

/**
  * @brief FLASH status of the last operation
  */
typedef enum
{
  FLASH_Status_Write_Protection_Error = (uint8_t)0x01, /*!< Write attempted to protected Block */
  FLASH_Status_TimeOut                = (uint8_t)0x02, /*!< Time out error */
  FLASH_Status_Successful_Operation   = (uint8_t)0x04  /*!< End of operation flag */
} FLASH_Status_TypeDef;

/**
  * @brief FLASH Power status during run and wait modes
  */
typedef enum
{
  FLASH_PowerStatus_IDDQDuringWaitMode          = (uint8_t)0x04, /*!< Flash program and data EEPROM
                                                                            in IDDQ during Wait mode*/
  FLASH_PowerStatus_IDDQDuringRunMode           = (uint8_t)0x08, /*!< Flash program and data EEPROM
                                                                            in IDDQ mode during Run mode*/
  FLASH_PowerStatus_IDDQDuringWaitAndRunModes   = (uint8_t)0x0C,  /*!<Flash program and data EEPROM
                                                                            in IDDQ during Wait and run modes*/
  FLASH_PowerStatus_On                          = (uint8_t)0x00  /*!< Flash program and data EEPROM
                                                                                    is powered on during Wait and Run modes */
} FLASH_PowerStatus_TypeDef;

/**
  * @brief FLASH flags definition
 * - Warning : FLAG value = mapping position register
  */
typedef enum {
  FLASH_FLAG_HVOFF     = (uint8_t)0x40,     /*!< End of high voltage flag */
  FLASH_FLAG_DUL       = (uint8_t)0x08,     /*!< Data EEPROM unlocked flag */
  FLASH_FLAG_EOP       = (uint8_t)0x04,     /*!< End of programming (write or erase operation) flag */
  FLASH_FLAG_PUL       = (uint8_t)0x02,     /*!< Flash Program memory unlocked flag */
  FLASH_FLAG_WR_PG_DIS = (uint8_t)0x01      /*!< Write attempted to protected page flag */
} FLASH_FLAG_TypeDef;


/**
  * @}
  */

/* Exported macros -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/

/** @addtogroup FLASH_Private_Macros
  * @{
  */

/**
  * @brief Macro used by the assert function in order to check the different
  *   sensitivity values for the flash Address
  */

#define IS_FLASH_PROGRAM_ADDRESS(Address) (((Address) >= FLASH_PROGRAM_START_PHYSICAL_ADDRESS) && \
    ((Address) <= FLASH_PROGRAM_END_PHYSICAL_ADDRESS))


/**
  * @brief Macro used by the assert function in order to check the different
  *   sensitivity values for the data Eeprom Address
  */

#define IS_FLASH_DATA_EEPROM_ADDRESS(Address) (((Address) >= FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS) && \
    ((Address) <= FLASH_DATA_EEPROM_END_PHYSICAL_ADDRESS))

/**
  * @brief Macro used by the assert function in order to check the different
  *  sensitivity values for the data eeprom and flash program Address
  */
#define IS_FLASH_ADDRESS(Address)((((Address) >= FLASH_PROGRAM_START_PHYSICAL_ADDRESS) && ((Address) <= FLASH_PROGRAM_END_PHYSICAL_ADDRESS)) || \
                                  (((Address) >= FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS) && ((Address) <= FLASH_DATA_EEPROM_END_PHYSICAL_ADDRESS)))

/**
  * @brief Macro used by the assert function in order to check the different
  *   sensitivity values for the option bytes Address
  */
#define IS_OPTION_BYTE_ADDRESS(ADDRESS) (((ADDRESS) >= FLASH_OPTION_BYTES_START_PHYSICAL_ADDRESS) && \
    ((ADDRESS) <= FLASH_OPTION_BYTES_END_PHYSICAL_ADDRESS))

/**
  * @brief Macro used by the assert function in order to check the different
  *   sensitivity values for the flash Block number
  */
#define IS_FLASH_PROGRAM_BLOCK_NUMBER(BlockNum) ((BlockNum) < FLASH_PROGRAM_BLOCKS_NUMBER)

/**
  * @brief Macro used by the assert function in order to check the different
  *   sensitivity values for the data eeprom Block number
  */
#define IS_FLASH_DATA_EEPROM_BLOCK_NUMBER(BlockNum) ((BlockNum) < FLASH_DATA_EEPROM_BLOCKS_NUMBER)

/**
  * @brief Macro used by the assert function in order to check the different
  *   sensitivity values for the flash memory type
  */
#define IS_FLASH_MEMORY_TYPE(MemType) (((MemType) == FLASH_MemType_Program) || \
                                       ((MemType) == FLASH_MemType_Data))

/**
  * @brief Macro used by the assert function in order to check the different
  *   sensitivity values for the flash program block mode
  */
#define IS_FLASH_PROGRAM_MODE(Mode) (((Mode) == FLASH_ProgramMode_Standard) || \
                                     ((Mode) == FLASH_ProgramMode_Fast))

/**
  * @brief Macro used by the assert function in order to check the program time mode
  */
#define IS_FLASH_PROGRAM_TIME(Time) (((Time) == FLASH_ProgramTime_Standard) || \
                                     ((Time) == FLASH_ProgramTime_TProg))

/**
  * @brief Macro used by the assert function in order to check the power mode
  */
#define IS_FLASH_POWER(Power) (((Power) == FLASH_Power_IDDQ) || \
                               ((Power) == FLASH_Power_On))

/**
  * @brief Macro used by the assert function in order to check the power status during wait and run modes
  */
#define IS_FLASH_POWERSTATUS(PowerStatus) (((PowerStatus) == FLASH_PowerStatus_IDDQDuringWaitMode) || \
    ((PowerStatus) == FLASH_PowerStatus_IDDQDuringRunMode ) || \
    ((PowerStatus) == FLASH_PowerStatus_IDDQDuringWaitAndRunModes) || \
    ((PowerStatus) == FLASH_Power_On))

/**
  * @brief Macro used by the assert function in order to check the different flags values
  */
#define IS_FLASH_FLAGS(FLAG) (((FLAG) == FLASH_FLAG_HVOFF) || \
                              ((FLAG) == FLASH_FLAG_DUL) || \
                              ((FLAG) == FLASH_FLAG_EOP) || \
                              ((FLAG) == FLASH_FLAG_PUL) || \
                              ((FLAG) == FLASH_FLAG_WR_PG_DIS))

/**
  * @}
  */

/* Exported functions ------------------------------------------------------- */
/** @addtogroup FLASH_Exported_Functions
  * @{
  */
void FLASH_DeInit(void);
void FLASH_Unlock(FLASH_MemType_TypeDef FLASH_MemType);
void FLASH_Lock(FLASH_MemType_TypeDef FLASH_MemType);
void FLASH_ITConfig(FunctionalState NewState);
void FLASH_ProgramByte(uint32_t Address, uint8_t Data);
void FLASH_EraseByte(uint32_t Address);
void FLASH_ProgramWord(uint32_t Address, uint32_t Data);
uint8_t FLASH_ReadByte(uint32_t Address);
void FLASH_ProgramOptionByte(uint16_t Address, uint8_t Data);
void FLASH_EraseOptionByte(uint16_t Address);
FLASH_ProgramTime_TypeDef FLASH_GetProgrammingTime(void);
void FLASH_SetProgrammingTime(FLASH_ProgramTime_TypeDef FLASH_ProgTime);
uint16_t FLASH_GetBootSize(void);
uint16_t FLASH_GetCodeSize(void);
FunctionalState FLASH_GetReadOutProtectionStatus(void);
void FLASH_PowerWaitModeConfig(FLASH_Power_TypeDef FLASH_Power);
FlagStatus FLASH_GetFlagStatus(FLASH_FLAG_TypeDef FLASH_FLAG);

/**
@code
 All the functions declared below must be executed from RAM exclusively, except 
 for the FLASH_WaitForLastOperation function which can be executed from Flash.
 
 Steps of the execution from RAM differs from one toolchain to another.
 for more details refer to stm8l15x_flash.c file.
 
 To enable execution from RAM you can either uncomment the following defines in the
 stm8l15x.h file or define them in your toolchain compiler preprocessor and though 
 depending on the used toolchain.
 - #define COSMIC_RAM_EXECUTION  (1)    with Cosmic
 - #define RAISONANCE_RAM_EXECUTION (1) with Raisonance
 - #define IAR_RAM_EXECUTION  (1)       with IAR 

@endcode
*/
FLASH_Status_TypeDef FLASH_WaitForLastOperation(FLASH_MemType_TypeDef FLASH_MemType) IN_RAM;
void FLASH_PowerRunModeConfig(FLASH_Power_TypeDef FLASH_Power) IN_RAM;
FLASH_PowerStatus_TypeDef FLASH_GetPowerStatus(void) IN_RAM;
void FLASH_ProgramBlock(uint16_t BlockNum, FLASH_MemType_TypeDef FLASH_MemType,
                        FLASH_ProgramMode_TypeDef FLASH_ProgMode, uint8_t *Buffer) IN_RAM;
void FLASH_EraseBlock(uint16_t BlockNum, FLASH_MemType_TypeDef FLASH_MemType) IN_RAM;


/**
  * @}
  */

#endif /*__STM8L15x_FLASH_H__*/

/**
  * @}
  */

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
