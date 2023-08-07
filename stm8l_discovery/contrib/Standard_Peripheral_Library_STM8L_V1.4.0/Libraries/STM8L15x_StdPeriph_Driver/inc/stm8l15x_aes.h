/**
  ********************************************************************************
  * @file    stm8l15x_aes.h
  * @author  MCD Application Team
  * @version V1.4.0
  * @date    09/24/2010
  * @brief   This file contains all the functions prototypes for the AES firmware
  *          library.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM8L15x_AES_H
#define __STM8L15x_AES_H

/* Includes ------------------------------------------------------------------*/
#include "stm8l15x.h"

/** @addtogroup STM8L15x_StdPeriph_Driver
  * @{
  */

/* Exported variables ------------------------------------------------------- */
/* Exported types ------------------------------------------------------------*/

/** @addtogroup AES_Exported_Types
  * @{
  */

/**
  * @brief AES possbile operation modes
  */
typedef enum
{
  AES_Operation_Encryp            = (uint8_t)0x00, /*!< AES in Encryption mode */
  AES_Operation_KeyDeriv          = (uint8_t)0x02, /*!< AES in Key Derivation mode */
  AES_Operation_Decryp            = (uint8_t)0x04, /*!< AES in Decryption mode */
  AES_Operation_KeyDerivAndDecryp = (uint8_t)0x06  /*!< AES in Key Derivation and Decryption mode */
} AES_Operation_TypeDef;

/**
  * @brief AES Flag possible values
  */
typedef enum
{
  AES_FLAG_CCF      = (uint8_t)0x01,  /*!< Computation Complete Flag */
  AES_FLAG_RDERR    = (uint8_t)0x02,  /*!< Read Error Flag           */
  AES_FLAG_WRERR    = (uint8_t)0x04   /*!< Write Error Flag          */
}AES_FLAG_TypeDef;

/**
  * @brief AES Interrupt sources possible values
  */
typedef enum
{
  AES_IT_CCIE   = (uint16_t)0x20, /*!< Computation Complete interrupt enable */
  AES_IT_ERRIE  = (uint16_t)0x40  /*!< Error interrupt enable                */
}AES_IT_TypeDef;

/**
  * @brief AES DMA transfer possible values
  */
typedef enum
{
  AES_DMATransfer_InOut = (uint8_t) 0x80 /*!< DMA requests enabled for input transfer phase
                                              as well as for the output transfer phase */
}
AES_DMATransfer_TypeDef;
/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/

/** @addtogroup AES_Private_Macros
  * @{
  */

/**
  * @brief Macro used by the assert function to check the different functions parameters.
  */

/**
  * @brief Macro used by the assert_param function in order to check the different
  *   flag values
  */
#define IS_AES_FLAG(Flag) \
  (((Flag) == AES_FLAG_CCF) || \
   ((Flag) == AES_FLAG_RDERR)  || \
   ((Flag) == AES_FLAG_WRERR))

/**
 * @brief Macro used by the assert function in order to check the different
 *  AES operation modes
 */
#define IS_AES_MODE(Operation) \
  (((Operation) == AES_Operation_Encryp) || \
   ((Operation) == AES_Operation_KeyDeriv)  || \
   ((Operation) == AES_Operation_Decryp) || \
   ((Operation) == AES_Operation_KeyDerivAndDecryp))

/**
 * @brief Macro used by the assert function in order to check the different
 *  IT sources values
 */
#define IS_AES_IT(IT) \
  (((IT) == AES_IT_CCIE) || \
   ((IT) == AES_IT_ERRIE))

/**
 * @brief Macro used by the assert function in order to check the different
 *  DMA transfer sources values
 */
#define IS_AES_DMATRANSFER(Transfer)   ((Transfer) == AES_DMATransfer_InOut)

/**
  * @}
  */

/* Exported functions ------------------------------------------------------- */
/** @addtogroup AES_Exported_functions
  * @{
  */

void AES_DeInit(void);
void AES_Config(AES_Operation_TypeDef AES_Operation);
void AES_Cmd(FunctionalState NewState);
void AES_ITConfig(AES_IT_TypeDef AES_IT, FunctionalState NewState);
void AES_WriteSubData(uint8_t Data);
void AES_WriteSubKey(uint8_t Key);
uint8_t AES_ReadSubData(void);
uint8_t AES_ReadSubKey(void);
void AES_DMAConfig(AES_DMATransfer_TypeDef AES_DMATransfer, FunctionalState NewState);
FlagStatus AES_GetFlagStatus(AES_FLAG_TypeDef AES_FLAG);
void AES_ClearFlag(AES_FLAG_TypeDef AES_FLAG);
ITStatus AES_GetITStatus(AES_IT_TypeDef AES_IT);
void AES_ClearITPendingBit(AES_IT_TypeDef AES_IT);

/**
  * @}
  */

#endif /* __STM8L15x_AES_H */

/**
  * @}
  */

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
