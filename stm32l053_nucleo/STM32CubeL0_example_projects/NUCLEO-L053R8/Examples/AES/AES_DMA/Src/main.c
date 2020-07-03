/**
  ******************************************************************************
  * @file    AES/AES_DMA/Src/main.c 
  * @author  MCD Application Team
  * @brief   This example provides a short description of how to use the AES
  *          peripheral to encrypt and decrypt data using AES Algorithm with ECB
  *          chaining mode, through the STM32L0xx HAL API.
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

/** @addtogroup AES_DMA
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* The size of the plaintext in Byte */
#define DATA_SIZE          ((uint32_t)64)
    
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* CRYP handler declaration */
CRYP_HandleTypeDef     CrypHandle;

/* Key size 128 bytes */
__ALIGN_BEGIN uint8_t aAES128Key[16] __ALIGN_END = 
                                 {0x2b,0x7e,0x15,0x16,0x28,0xae,0xd2,0xa6, 
                                  0xab,0xf7,0x15,0x88,0x09,0xcf,0x4f,0x3c};
                               
/* Plaintext */                
uint8_t aPlaintext[DATA_SIZE]  = {0x6b,0xc1,0xbe,0xe2,0x2e,0x40,0x9f,0x96,
                                  0xe9,0x3d,0x7e,0x11,0x73,0x93,0x17,0x2a,
                                  0xae,0x2d,0x8a,0x57,0x1e,0x03,0xac,0x9c,
                                  0x9e,0xb7,0x6f,0xac,0x45,0xaf,0x8e,0x51,
                                  0x30,0xc8,0x1c,0x46,0xa3,0x5c,0xe4,0x11,
                                  0xe5,0xfb,0xc1,0x19,0x1a,0x0a,0x52,0xef,
                                  0xf6,0x9f,0x24,0x45,0xdf,0x4f,0x9b,0x17,
                                  0xad,0x2b,0x41,0x7b,0xe6,0x6c,0x37,0x10};
                               
/* Cyphertext */               
uint8_t aCyphertext[DATA_SIZE] = {0x3A,0xD7,0x7B,0xB4,0x0D,0x7A,0x36,0x60,
                                  0xA8,0x9E,0xCA,0xF3,0x24,0x66,0xEF,0x97,
                                  0xF5,0xD3,0xD5,0x85,0x03,0xB9,0x69,0x9D,
                                  0xE7,0x85,0x89,0x5A,0x96,0xFD,0xBA,0xAF,
                                  0x43,0xB1,0xCD,0x7F,0x59,0x8E,0xCE,0x23,
                                  0x88,0x1B,0x00,0xE3,0xED,0x03,0x06,0x88,
                                  0x7B,0x0C,0x78,0x5E,0x27,0xE8,0xAD,0x3F,
                                  0x82,0x23,0x20,0x71,0x04,0x72,0x5D,0xD4};
/* Encrypted text */  
uint8_t aEncryptedText[DATA_SIZE];

/* Decrypted text */
uint8_t aDecryptedText[DATA_SIZE];

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void Error_Handler(void);

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

  /* Initialize LED2 */
  BSP_LED_Init(LED2);
    
  /* Configure the System clock to have a frequency of 2 MHz (Up to 32MHZ possible) */
  SystemClock_Config();

  
  /*##-1- Configure the CRYP (AES) peripheral ################################*/
  /* Deinitialize Crypto peripheral */
  HAL_CRYP_DeInit(&CrypHandle);

  /* Set the CRYP parameters */
  CrypHandle.Instance = AES;
  CrypHandle.Init.DataType = CRYP_DATATYPE_8B;
  CrypHandle.Init.pKey     = aAES128Key;
  
  if(HAL_CRYP_Init(&CrypHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler(); 
  }
  
  /*##-2- Start the AES encryption in ECB chaining mode with DMA #############*/
  if(HAL_CRYP_AESECB_Encrypt_DMA(&CrypHandle, aPlaintext, DATA_SIZE, aEncryptedText) != HAL_OK)
  {
    /* Processing Error */
    Error_Handler(); 
  }
  
  /*  Before starting a new process, you need to check the current state of the peripheral; 
      if it’s busy you need to wait for the end of current transfer before starting a new one.
      For simplicity reasons, this example is just waiting till the end of the 
      process, but application may perform other tasks while transfer operation
      is ongoing. */ 
  while (HAL_CRYP_GetState(&CrypHandle) != HAL_CRYP_STATE_READY)
  {
  }
  
  /*##-3- Check the encrypted text with the expected one #####################*/
  if(memcmp(aEncryptedText, aCyphertext, DATA_SIZE) != 0)
  {
    /* Wrong encryption: Turn LED2 on */
    BSP_LED_On(LED2);
  }
  else
  {
    /* Right encryption */
  }
  
  /* Deinitialize Crypto peripheral */
  HAL_CRYP_DeInit(&CrypHandle);
  
  /* Set the CRYP parameters */
  CrypHandle.Instance = AES;
  CrypHandle.Init.DataType = CRYP_DATATYPE_8B;
  CrypHandle.Init.pKey     = aAES128Key;
  
  if(HAL_CRYP_Init(&CrypHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler(); 
  }
  
  /*##-4- Start the AES decryption in ECB chaining mode with DMA #############*/
  if(HAL_CRYP_AESECB_Decrypt_DMA(&CrypHandle, aEncryptedText, DATA_SIZE, aDecryptedText) != HAL_OK)
  {
    /* Processing Error */
    Error_Handler(); 
  }
  
  /*  Before starting a new process, you need to check the current state of the peripheral; 
      if it’s busy you need to wait for the end of current transfer before starting a new one.
      For simplicity reasons, this example is just waiting till the end of the 
      process, but application may perform other tasks while transfer operation
      is ongoing. */ 
  while (HAL_CRYP_GetState(&CrypHandle) != HAL_CRYP_STATE_READY)
  {
  }
  
  /*##-5- Check the decrypted text with the expected one #####################*/
  if(memcmp(aDecryptedText, aPlaintext, DATA_SIZE) != 0)
  {
    /* Wrong encryption: Turn LED2 on */
    BSP_LED_On(LED2);
  }
  else
  {
    /* Right encryption */
  }
  
  /* Infinite loop */
  while(1)
  {
  }
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
  RCC_OscInitStruct.MSICalibrationValue=0x00;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
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
    /* Initialization Error */
    Error_Handler();
  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  /* Turn LED2 on */
  BSP_LED_On(LED2);
  while(1)
  {
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
