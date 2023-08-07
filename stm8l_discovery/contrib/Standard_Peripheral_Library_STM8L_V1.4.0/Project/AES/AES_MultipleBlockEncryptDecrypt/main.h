/**
  ******************************************************************************
  * @file    AES/AES_MultipleBlockEncryptDecrypt/main.h
  * @author  MCD Application Team
  * @version V1.5.2
  * @date    30-September-2014
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */  

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#include "stm8l15x.h"
#include "stm8_eval.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define AES_DINR_ADDRESS            (uint16_t)0x53D2
#define AES_DOUTR_ADDRESS           (uint16_t)0x53D3

/* Private macro -------------------------------------------------------------*/
#define countof(a)        (sizeof(a) / sizeof(*(a)))
#define PLAINTEXT_SIZE    (uint8_t)(countof(PlainText)-1)

/* Private variables ---------------------------------------------------------*/
/* Data to be encrypted: data length must be multiple of 16 bytes and lower than 112 bytes */
NEAR uint8_t PlainText[] = "Four 8-bit product lines combine architecture with ST’s EnergyLite™ technology to do more work with less energy.";
NEAR uint8_t  CypherText[PLAINTEXT_SIZE + 1] = {0};
NEAR uint8_t  ComputedPlainText[PLAINTEXT_SIZE + 1] = {0};
NEAR uint8_t  SrcBuffer[PLAINTEXT_SIZE * 2 + 2] = {0};
/* encryption key used to encrypt PlainText */
uint8_t EncryptionKey[16] = "ultra low-power";

/* Private function prototypes -----------------------------------------------*/
ErrorStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
