/**
  ******************************************************************************
  * @file    AES/AES_Modes/Src/main.c 
  * @author  MCD Application Team
  * @brief   This example provides a short description of how to use the CRYPTO
  *          peripheral to encrypt and decrypt data using AES in chaining modes
  *          (ECB, CBC, CTR) and key size 128 Algorithm.
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
#include "main.h"

/** @addtogroup STM32L0xx_HAL_Examples
  * @{
  */

/** @addtogroup AES_Modes
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/    
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* CRYP handler declaration */
CRYP_HandleTypeDef     CrypHandle;
  
/* Initialization vector */
uint8_t aInitVector[16] = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,
                           0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f}; 
/* Key size 128 bytes */
__ALIGN_BEGIN uint8_t aAES128Key[16] __ALIGN_END =
													        {0x2b,0x7e,0x15,0x16,0x28,0xae,0xd2,0xa6, 
                                  0xab,0xf7,0x15,0x88,0x09,0xcf,0x4f,0x3c};

/* Plaintext */
uint8_t aPlaintext[AES_TEXT_SIZE] = {0x6b,0xc1,0xbe,0xe2,0x2e,0x40,0x9f,0x96,
                                     0xe9,0x3d,0x7e,0x11,0x73,0x93,0x17,0x2a,
                                     0xae,0x2d,0x8a,0x57,0x1e,0x03,0xac,0x9c,
                                     0x9e,0xb7,0x6f,0xac,0x45,0xaf,0x8e,0x51,
                                     0x30,0xc8,0x1c,0x46,0xa3,0x5c,0xe4,0x11,
                                     0xe5,0xfb,0xc1,0x19,0x1a,0x0a,0x52,0xef,
                                     0xf6,0x9f,0x24,0x45,0xdf,0x4f,0x9b,0x17,
                                     0xad,0x2b,0x41,0x7b,0xe6,0x6c,0x37,0x10};

/* Cyphertext */
uint8_t aCyphertext[AES_TEXT_SIZE] = {0x76,0x49,0xab,0xac,0x81,0x19,0xb2,0x46,
                                      0xce,0xe9,0x8e,0x9b,0x12,0xe9,0x19,0x7d,
                                      0x50,0x86,0xcb,0x9b,0x50,0x72,0x19,0xee,
                                      0x95,0xdb,0x11,0x3a,0x91,0x76,0x78,0xb2,
                                      0x73,0xbe,0xd6,0xb8,0xe3,0xc1,0x74,0x3b,
                                      0x71,0x16,0xe6,0x9e,0x22,0x22,0x95,0x16,
                                      0x3f,0xf1,0xca,0xa1,0x68,0x1f,0xac,0x09,
                                      0x12,0x0e,0xca,0x30,0x75,0x86,0xe1,0xa7};

#ifdef AES_ECB  
/* Expected text: Encrypted Data with AES 128 Mode ECB */
uint8_t aEncryptedTextECB[AES_TEXT_SIZE] = {0x3A, 0xD7, 0x7B, 0xB4, 0x0D, 0x7A,
                                            0x36, 0x60, 0xA8, 0x9E, 0xCA, 0xF3,
                                            0x24, 0x66, 0xEF, 0x97, 0xF5, 0xD3,
                                            0xD5, 0x85, 0x03, 0xB9, 0x69, 0x9D,
                                            0xE7, 0x85, 0x89, 0x5A, 0x96, 0xFD,
                                            0xBA, 0xAF, 0x43, 0xB1, 0xCD, 0x7F,
                                            0x59, 0x8E, 0xCE, 0x23, 0x88, 0x1B,
                                            0x00, 0xE3, 0xED, 0x03, 0x06, 0x88, 
                                            0x7B, 0x0C, 0x78, 0x5E, 0x27, 0xE8, 
                                            0xAD, 0x3F, 0x82, 0x23, 0x20, 0x71, 
                                            0x04, 0x72, 0x5D, 0xD4};

/* Expected text: Decrypted Data with AES 128 Mode ECB */
uint8_t aDecryptedTextECB[AES_TEXT_SIZE] = {0x6B, 0xC0, 0xBC, 0xE1, 0x2A, 0x45, 
                                            0x99, 0x91, 0xE1, 0x34, 0x74, 0x1A, 
                                            0x7F, 0x9E, 0x19, 0x25, 0xD8, 0x64, 
                                            0x21, 0xFB, 0x9F, 0x1A, 0x1E, 0xDA, 
                                            0x50, 0x5E, 0xE1, 0x37, 0x57, 0x46, 
                                            0x97, 0x2C, 0x60, 0x4E, 0xD7, 0xDD, 
                                            0xF3, 0x2E, 0xFD, 0xFF, 0x70, 0x20, 
                                            0xD0, 0x23, 0x8B, 0x7C, 0x2A, 0x5D,
                                            0x85, 0x21, 0xF2, 0xFD, 0x3C, 0x8E, 
                                            0xEF, 0x2C, 0xDC, 0x3D, 0xA7, 0xE5, 
                                            0xC4, 0x4E, 0xA2, 0x06}; 
#endif /* AES_ECB */ 

#ifdef AES_CBC  
/* Expected text: Encrypted Data with AES 128 Mode CBC */
uint8_t aDecryptedTextCBC[AES_TEXT_SIZE] = {0x6B, 0xC1, 0xBE, 0xE2, 0x2E, 0x40, 
                                            0x9F, 0x96, 0xE9, 0x3D, 0x7E, 0x11, 
                                            0x73, 0x93, 0x17, 0x2A, 0xAE, 0x2D, 
                                            0x8A, 0x57, 0x1E, 0x03, 0xAC, 0x9C, 
                                            0x9E, 0xB7, 0x6F, 0xAC, 0x45, 0xAF, 
                                            0x8E, 0x51, 0x30, 0xC8, 0x1C, 0x46, 
                                            0xA3, 0x5C, 0xE4, 0x11, 0xE5, 0xFB, 
                                            0xC1, 0x19, 0x1A, 0x0A, 0x52, 0xEF,
                                            0xF6, 0x9F, 0x24, 0x45, 0xDF, 0x4F, 
                                            0x9B, 0x17, 0xAD, 0x2B, 0x41, 0x7B, 
                                            0xE6, 0x6C, 0x37, 0x10};
#endif /* AES_CBC */

#ifdef AES_CTR  
/* Expected text: Encrypted Data with AES 128 Mode CTR */
uint8_t aEncryptedTextCTR[AES_TEXT_SIZE] = {0x3B, 0x3F, 0xD9, 0x2E, 0xB7, 0x2D, 
                                            0xAD, 0x20, 0x33, 0x34, 0x49, 0xF8, 
                                            0xE8, 0x3C, 0xFB, 0x4A, 0x01, 0x0C, 
                                            0x04, 0x19, 0x99, 0xE0, 0x3F, 0x36, 
                                            0x44, 0x86, 0x24, 0x48, 0x3E, 0x58, 
                                            0x2D, 0x0E, 0xA6, 0x22, 0x93, 0xCF, 
                                            0xA6, 0xDF, 0x74, 0x53, 0x5C, 0x35, 
                                            0x41, 0x81, 0x16, 0x87, 0x74, 0xDF,
                                            0x2D, 0x55, 0xA5, 0x47, 0x06, 0x27, 
                                            0x3C, 0x50, 0xD7, 0xB4, 0xF8, 0xA8, 
                                            0xCD, 0xDC, 0x6E, 0xD7};

/* Expected text: Decrypted Data with AES 128 Mode CTR */
uint8_t aDecryptedTextCTR[AES_TEXT_SIZE] = {0x26, 0xB7, 0xCC, 0x60, 0x18, 0x74, 
                                            0x80, 0xF0, 0x14, 0xE0, 0xB9, 0x72, 
                                            0x89, 0x46, 0xF5, 0x1D, 0xFF, 0xA7, 
                                            0x45, 0xD5, 0xD7, 0x91, 0x8A, 0x44,
                                            0x4F, 0xEA, 0x5A, 0xDE, 0xEA, 0x81, 
                                            0xDB, 0xED, 0xE5, 0x54, 0x59, 0x31, 
                                            0xE6, 0x42, 0xE4, 0x79, 0xC8, 0xD8, 
                                            0x66, 0x06, 0x2E, 0xAF, 0xB3, 0x26,
                                            0xE4, 0x3B, 0x4B, 0xA3, 0xB1, 0x77, 
                                            0x0B, 0x4E, 0x68, 0x91, 0x73, 0xE3, 
                                            0x5E, 0x36, 0xB8, 0x60};
#endif /* AES_CTR */

/* Used for storing the encrypted text */
uint8_t aEncryptedtext[AES_TEXT_SIZE];

/* Used for storing the decrypted text */
uint8_t aDecryptedtext[AES_TEXT_SIZE];

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void ErrorHandler(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /* STM32L0xx HAL library initialization:
       - Configure the Flash prefetch, Flash preread and Buffer caches
       - Systick timer is configured by default as source of time base, but user 
             can eventually implement his proper time base source (a general purpose 
             timer for example or other time source), keeping in mind that Time base 
             duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and 
             handled in milliseconds basis.
       - Low Level Initialization
     */
  
  HAL_Init();
  
  /* Configure the system clock */
  SystemClock_Config();
  
  /* Configure Key Button */
  BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_GPIO);
  
  /* Configures LED */
  BSP_LED_Init(LED2); 
  /*##-1- Configure the CRYP peripheral ######################################*/
  /* Set the common CRYP parameters */
  CrypHandle.Init.DataType = CRYP_DATATYPE_8B;
  
  if(HAL_CRYP_Init(&CrypHandle) != HAL_OK)
  {
    /* Initialization Error */
    ErrorHandler(); 
  }
  
/******************************************************************************/
/*                             AES mode ECB                                   */
/******************************************************************************/
#ifdef AES_ECB
/*=====================================================
  Encryption ECB mode                                        
======================================================*/
  if (HAL_CRYP_DeInit(&CrypHandle) != HAL_OK)
  {
    ErrorHandler();
  }    
/*****************  AES 128   ****************/
  /* Initialize the CRYP peripheral */
  CrypHandle.Instance = AES;
  CrypHandle.Init.pKey = aAES128Key;
  
  if(HAL_CRYP_Init(&CrypHandle) != HAL_OK)
  {
    /* Error */
    ErrorHandler(); 
  }
  
  /* Start encrypting aPlaintext, the cypher data is available in aEncryptedtext */
  if (HAL_CRYP_AESECB_Encrypt(&CrypHandle, aPlaintext, AES_TEXT_SIZE, aEncryptedtext, TIMEOUT_VALUE) != HAL_OK)
  {
    /* Processing Error */
    ErrorHandler(); 
  }  
  /* Check the encrypted text with the expected one *************************/    
  if(memcmp(aEncryptedtext, aEncryptedTextECB, AES_TEXT_SIZE ) != 0)
  {
    /* Initialization Error */
    ErrorHandler(); 
  }
  
  /* Wait until Key button is pressed to enter the next mode */
  while(BSP_PB_GetState(BUTTON_KEY) != RESET)
  {
  }
  /* Loop while Key button is maintained pressed */
  while(BSP_PB_GetState(BUTTON_KEY) == RESET)
  {
  }
  
/*=====================================================
  Decryption ECB mode                                        
======================================================*/
  if (HAL_CRYP_DeInit(&CrypHandle) != HAL_OK)
  {
    ErrorHandler();
  } 
/*****************  AES 128   ****************/
  /* Initialize the CRYP peripheral */
  CrypHandle.Instance = AES;
  CrypHandle.Init.pKey    = aAES128Key;
  
  if(HAL_CRYP_Init(&CrypHandle) != HAL_OK)
  {
    /* Error */
    ErrorHandler(); 
  }
  
  /* Start decrypting aCyphertext, the decrypted data is available in aDecryptedtext */
  if (HAL_CRYP_AESECB_Decrypt(&CrypHandle, aCyphertext, AES_TEXT_SIZE, aDecryptedtext, TIMEOUT_VALUE) != HAL_OK)
  {
    /* Processing Error */
    ErrorHandler(); 
  }   
  
  /* Check the decrypted text with the expected one *************************/    
  if(memcmp(aDecryptedtext, aDecryptedTextECB, AES_TEXT_SIZE ) != 0)
  {
    /* Initialization Error */
    ErrorHandler(); 
  }
  
#endif /* AES_ECB */   
  
/******************************************************************************/
/*                             AES mode CBC                                   */
/******************************************************************************/
#ifdef AES_CBC    
/*=====================================================
  Encryption CBC mode                                        
======================================================*/
  if (HAL_CRYP_DeInit(&CrypHandle) != HAL_OK)
  {
    ErrorHandler();
  }
/*****************  AES 128   ****************/
  /* Initialize the CRYP peripheral */
  CrypHandle.Instance = AES;
  CrypHandle.Init.pKey      = aAES128Key;
  CrypHandle.Init.pInitVect = aInitVector;
  
  if(HAL_CRYP_Init(&CrypHandle) != HAL_OK)
  {
    /* Error */
    ErrorHandler(); 
  }
  
  /* Start encrypting aPlaintext, the cypher data is available in aEncryptedtext */
  if (HAL_CRYP_AESCBC_Encrypt(&CrypHandle, aPlaintext, AES_TEXT_SIZE, aEncryptedtext, TIMEOUT_VALUE)!= HAL_OK)
  {
    /* Processing Error */
    ErrorHandler(); 
  } 
  
  /* Check the encrypted text with the expected one *************************/    
  if(memcmp(aEncryptedtext, aCyphertext, AES_TEXT_SIZE ) != 0)
  {
    /* Initialization Error */
    ErrorHandler(); 
  }
  
  /* Wait until Key button is pressed to enter the next mode */
  while(BSP_PB_GetState(BUTTON_KEY) != RESET)
  {
  }
  /* Loop while Key button is maintained pressed */
  while(BSP_PB_GetState(BUTTON_KEY) == RESET)
  {
  }
  
/*=====================================================
  Decryption CBC mode                                        
======================================================*/
  if (HAL_CRYP_DeInit(&CrypHandle) != HAL_OK)
  {
    ErrorHandler();
  }   
/*****************  AES 128   ****************/
  /* Initialize the CRYP peripheral */
  CrypHandle.Instance = AES;
  CrypHandle.Init.pKey      = aAES128Key;
  CrypHandle.Init.pInitVect = aInitVector;
  
  if(HAL_CRYP_Init(&CrypHandle) != HAL_OK)
  {
    /* Error */
    ErrorHandler(); 
  }
  
  /* Start decrypting aCyphertext, the decrypted data is available in aDecryptedtext */
  if (HAL_CRYP_AESCBC_Decrypt(&CrypHandle, aCyphertext, AES_TEXT_SIZE, aDecryptedtext, TIMEOUT_VALUE) != HAL_OK)
  {
    /* Processing Error */
    ErrorHandler(); 
  }
  /* Check the decrypted text with the expected one *************************/    
  if(memcmp(aDecryptedtext, aDecryptedTextCBC, AES_TEXT_SIZE ) != 0)
  {
    /* Error */
    ErrorHandler(); 
  }
#endif /* AES_CBC */  
/******************************************************************************/
/*                             AES mode CTR                                   */
/******************************************************************************/
  
#ifdef AES_CTR
/*=====================================================
  Encryption CTR mode                                        
======================================================*/
  if (HAL_CRYP_DeInit(&CrypHandle) != HAL_OK)
  {
    ErrorHandler();
  }  
  
/*****************  AES 128   ****************/
  /* Initialize the CRYP peripheral */
  CrypHandle.Instance = AES;
  CrypHandle.Init.pKey      = aAES128Key;
  CrypHandle.Init.pInitVect = aInitVector;
  
  if(HAL_CRYP_Init(&CrypHandle) != HAL_OK)
  {
    /* Error */
    ErrorHandler(); 
  }
  
  /* Start encrypting aPlaintext, the cypher data is available in aEncryptedtext */
  if (HAL_CRYP_AESCTR_Encrypt(&CrypHandle, aPlaintext, AES_TEXT_SIZE, aEncryptedtext, TIMEOUT_VALUE) != HAL_OK)
  {
    /* Processing Error */
    ErrorHandler(); 
  }
  
  /* Check the encrypted text with the expected one *************************/    
  if(memcmp(aEncryptedtext, aEncryptedTextCTR, AES_TEXT_SIZE ) != 0)
  {
    /* Error */
    ErrorHandler(); 
  }
  
  /* Wait until Key button is pressed to enter the next mode */
  while(BSP_PB_GetState(BUTTON_KEY) != RESET)
  {
  }
  /* Loop while Key button is maintained pressed */
  while(BSP_PB_GetState(BUTTON_KEY) == RESET)
  {
  }
/*=====================================================
  Decryption CTR mode                                        
======================================================*/
  if (HAL_CRYP_DeInit(&CrypHandle) != HAL_OK)
  {
    ErrorHandler();
  }    
/*****************  AES 128   ****************/
  /* Initialize the CRYP peripheral */
  CrypHandle.Instance = AES;
  CrypHandle.Init.pKey      = aAES128Key;
  CrypHandle.Init.pInitVect = aInitVector;
  
  if(HAL_CRYP_Init(&CrypHandle) != HAL_OK)
  {
    /* Error */
    ErrorHandler(); 
  }
  
  /* Start decrypting aCyphertext, the decrypted data is available in aDecryptedtext */
  if (HAL_CRYP_AESCTR_Decrypt(&CrypHandle, aCyphertext, AES_TEXT_SIZE, aDecryptedtext, TIMEOUT_VALUE) != HAL_OK)
  {
    /* Processing Error */
    ErrorHandler(); 
  }
  
  /* Check the decrypted text with the expected one *************************/    
  if(memcmp(aDecryptedtext, aDecryptedTextCTR, AES_TEXT_SIZE ) != 0)
  {
    /* Error */
    ErrorHandler(); 
  }
#endif /* AES_CTR */
  
  /* Turn LED2 on */
  BSP_LED_On(LED2);
  
  /* Infinite loop */ 
  while(1)
  {} 
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = MSI
  *            SYSCLK(Hz)                     = 2000000
  *            HCLK(Hz)                       = 2000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            APB2 Prescaler                 = 1
  *            Flash Latency(WS)              = 0
  *            Main regulator output voltage  = Scale3 mode
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  
  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();
  
  /* The voltage scaling allows optimizing the power consumption when the device is 
     clocked below the maximum system frequency, to update the voltage scaling value 
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);
  
  /* Enable MSI Oscillator */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_5;
  RCC_OscInitStruct.MSICalibrationValue = 0x00;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Error */
    ErrorHandler();
  }
  
  
  /* Select MSI as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;  
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    /* Error */
    ErrorHandler();
  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void ErrorHandler(void)
{
  while(1)
  {
    /* Toggle LED2 */
    BSP_LED_Toggle(LED2);
    HAL_Delay(200);
  }
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */ 

/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
