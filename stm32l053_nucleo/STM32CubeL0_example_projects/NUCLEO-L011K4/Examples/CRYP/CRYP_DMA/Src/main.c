/**
  ******************************************************************************
  * @file    CRYP/CRYP_DMA/Src/main.c
  * @author  MCD Application Team
  * @brief   This example provides a short description of how to use the CRYPTO
  *          peripheral to encrypt and decrypt data using CRYP Algorithm with
  *          AES ECB chaining mode.
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

/** @addtogroup CRYP_DMA
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

#define KEY_SIZE          128 /* Key size in bits */

/* The size of the plaintext in Byte */
#define PLAINTEXT_SIZE          ((uint32_t)64)
#define AES_TEXT_SIZE           ((uint32_t)64)

#define ECB               1
#define CBC               2
#define CTR               3

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* CRYP handler declaration */
CRYP_HandleTypeDef     CrypHandle;

/* UART handler declaration */
UART_HandleTypeDef     UartHandle;

/* Key size 128 bits */
uint8_t aAES128Key[16] = 
                         {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
                          0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c
                         };

/* Plaintext */
uint8_t aPlaintext[PLAINTEXT_SIZE]  =    {0x6b,0xc1,0xbe,0xe2,0x2e,0x40,0x9f,0x96,
                                     0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a,
                                     0xae, 0x2d, 0x8a, 0x57, 0x1e, 0x03, 0xac, 0x9c,
                                     0x9e, 0xb7, 0x6f, 0xac, 0x45, 0xaf, 0x8e, 0x51,
                                     0x30, 0xc8, 0x1c, 0x46, 0xa3, 0x5c, 0xe4, 0x11,
                                     0xe5, 0xfb, 0xc1, 0x19, 0x1a, 0x0a, 0x52, 0xef,
                                     0xf6, 0x9f, 0x24, 0x45, 0xdf, 0x4f, 0x9b, 0x17,
                                     0xad, 0x2b, 0x41, 0x7b, 0xe6, 0x6c, 0x37, 0x10
                                    };
                                      
/* Expected text: Encrypted Data with AES 128 Mode ECB */
uint8_t aEncryptedtextExpected[AES_TEXT_SIZE] = {0x3A, 0xD7, 0x7B, 0xB4, 0x0D, 0x7A,
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

/* Used for storing the encrypted text */
uint8_t aEncryptedText[AES_TEXT_SIZE] = {0};

/* Used for storing the decrypted text */
uint8_t aDecryptedText[PLAINTEXT_SIZE] = {0};

/**
  * @brief COM variables
  */
USART_TypeDef* COM_USART[COMn]  = {BSP_COM1}; 

GPIO_TypeDef* COM_TX_PORT[COMn] = {BSP_COM1_TX_GPIO_PORT};
 
GPIO_TypeDef* COM_RX_PORT[COMn] = {BSP_COM1_RX_GPIO_PORT};

const uint16_t COM_TX_PIN[COMn] = {BSP_COM1_TX_PIN};

const uint16_t COM_RX_PIN[COMn] = {BSP_COM1_RX_PIN};

const uint16_t COM_TX_AF[COMn]  = {BSP_COM1_TX_AF};

const uint16_t COM_RX_AF[COMn]  = {BSP_COM1_RX_AF};

/* Private function prototypes -----------------------------------------------*/
#ifdef __GNUC__
/* With GCC, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

static void PressToContinue(void);

void SystemClock_Config(void);
static void Error_Handler(void);
static void Display_PlainData(uint32_t datalength);
static void Display_EncryptedData(uint8_t mode, uint16_t keysize, uint32_t datalength);
static void Display_DecryptedData(uint8_t mode, uint16_t keysize, uint32_t datalength);
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

  /* Configure the system clock to 2 MHz */
  SystemClock_Config();
  
  /* Configure the COM port */
  UartHandle.Init.BaudRate = 115200;
  UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
  UartHandle.Init.StopBits = UART_STOPBITS_1;
  UartHandle.Init.Parity = UART_PARITY_NONE;
  UartHandle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  UartHandle.Init.Mode = UART_MODE_TX_RX;
  BSP_COM_Init(COM1, &UartHandle);

  /* Configures LED */
  BSP_LED_Init(LED3);

   /*#######################################################################*/
   /*                                                                       */
   /*##-         DMA-based AES 128 ECB encryption              #############*/
   /*                                                                       */   
   /*#######################################################################*/
  /*##- Configure the CRYP peripheral ######################################*/
  
  CrypHandle.Instance = AES;
  CrypHandle.Init.DataType      = CRYP_DATATYPE_8B;
  CrypHandle.Init.pKey          = aAES128Key;

  if (HAL_CRYP_Init(&CrypHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  
  /* Display Plain Data*/
  Display_PlainData(PLAINTEXT_SIZE);
	
    PressToContinue();
  BSP_LED_Off(LED3);



  if(HAL_CRYP_AESECB_Encrypt_DMA(&CrypHandle, aPlaintext, PLAINTEXT_SIZE, aEncryptedText) == HAL_OK)   
  {
      /* Display encrypted Data */
      Display_EncryptedData(ECB, 128, PLAINTEXT_SIZE);
  }
  else
  {
    /* Processing Error */
    Error_Handler(); 
  }

  /*  Before starting a new process, the current state of the peripheral is checked; 
      as long as the state is not set back to READY, no new ciphering processing
      can be started.
      For simplicity's sake, this example is just waiting till the end of the 
      process, but application may perform other tasks while cihering operation
      is ongoing. */ 
  while (HAL_CRYP_GetState(&CrypHandle) != HAL_CRYP_STATE_READY)
  {
  }
  
  
  /*##- Compare the encrypted text with the expected one #####################*/
  if(memcmp(aEncryptedText, aEncryptedtextExpected, PLAINTEXT_SIZE) != 0)
  {
	 /* Wrong encryption */
     BSP_LED_On(LED3);
  }
  else
  {
    /* Correct encryption */
  }
  
  
   /*#######################################################################*/
   /*                                                                       */
   /*##-         DMA-based AES 128 ECB decryption              #############*/
   /*                                                                       */   
   /*#######################################################################*/  
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
  
  if(HAL_CRYP_AESECB_Decrypt_DMA(&CrypHandle, aEncryptedText, PLAINTEXT_SIZE, aDecryptedText) == HAL_OK)    
  {
      /* Display Decrypted Data */
      Display_DecryptedData(ECB, 128, PLAINTEXT_SIZE);
  }
  else
  {
    /* Processing Error */
    Error_Handler(); 
  }
  
  /*  Before starting a new process, the current state of the peripheral is checked; 
      as long as the state is not set back to READY, no new ciphering processing
      can be started.
      For simplicity's sake, this example is just waiting till the end of the 
      process, but application may perform other tasks while cihering operation
      is ongoing. */ 
  while (HAL_CRYP_GetState(&CrypHandle) != HAL_CRYP_STATE_READY)
  {
  }
  

  /*##- Compare the decrypted text with the expected one #####################*/
  if(memcmp(aDecryptedText, aPlaintext, PLAINTEXT_SIZE) != 0)
  {
  
	/* Wrong encryption */
    BSP_LED_On(LED3);
  }
  else
  {
    /* Correct decryption */
  }
  
  printf("======================================================\n");
  printf("\n\r DMA-based AES 128 ECB encryption/decryption done.\n");
  printf("\n\r No issue detected.\n");

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
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  
  /* Enable MSI Oscillator */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_5;
  RCC_OscInitStruct.MSICalibrationValue=0x00;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct)!= HAL_OK)
  {
    /* Initialization Error */
    while(1); 
  }
  
  /* Select MSI as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;  
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0)!= HAL_OK)
  {
    /* Initialization Error */
    while(1); 
  }
  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();
  
  /* The voltage scaling allows optimizing the power consumption when the device is 
     clocked below the maximum system frequency, to update the voltage scaling value 
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);
  
}


/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  while(1)
  {
    /* Toggle LED */
    BSP_LED_Toggle(LED3);
    HAL_Delay(200);
  }

}

/**
  * @brief  Display Plain Data
  * @param  datalength: length of the data to display
  * @retval None
  */
static void Display_PlainData(uint32_t datalength)
{
  uint32_t BufferCounter = 0;
  uint32_t count = 0;

  printf("\n\r =============================================================\n\r");
  printf(" ================== Crypt Using HW Cryp  =====================\n\r");
  printf(" =============================================================\n\r");
  printf(" ---------------------------------------\n\r");
  printf(" Plain Data :\n\r");
  printf(" ---------------------------------------\n\r");

  for (BufferCounter = 0; BufferCounter < datalength; BufferCounter++)
  {
    printf("[0x%02X]", aPlaintext[BufferCounter]);
    count++;

    if (count == 16)
    {
      count = 0;
      printf("  Block %lu \n\r", BufferCounter / 16);
    }
  }
}

/**
  * @brief  Display Encrypted Data
  * @param  mode: chaining mode
  * @param  keysize: AES key size used
  * @param  datalength: length of the data to display
  * @retval None
  */
static void Display_EncryptedData(uint8_t mode, uint16_t keysize, uint32_t datalength)
{
  uint32_t BufferCounter = 0;
  uint32_t count = 0;

  printf("\n\r =======================================\n\r");
  printf(" Encrypted Data with AES %d  Mode  ", keysize);

  if (mode == ECB)
  {
    printf("ECB\n\r");
  }
  else if (mode == CBC)
  {
    printf("CBC\n\r");
  }
  else /* if(mode == CTR)*/
  {
    printf("CTR\n\r");
  }

  printf(" ---------------------------------------\n\r");

  for (BufferCounter = 0; BufferCounter < datalength; BufferCounter++)
  {
    printf("[0x%02X]", aEncryptedText[BufferCounter]);

    count++;
    if (count == 16)
    {
      count = 0;
      printf(" Block %lu \n\r", BufferCounter / 16);
    }
  }
}

/**
  * @brief  Display Decrypted Data
  * @param  mode: chaining mode
  * @param  keysize: AES key size used
  * @param  datalength: length of the data to display
  * @retval None
  */
static void Display_DecryptedData(uint8_t mode, uint16_t keysize, uint32_t datalength)
{
  uint32_t BufferCounter = 0;
  uint32_t count = 0;

  printf("\n\r =======================================\n\r");
  printf(" Decrypted Data with AES %d  Mode  ", keysize);

  if (mode == ECB)
  {
    printf("ECB\n\r");
  }
  else if (mode == CBC)
  {
    printf("CBC\n\r");
  }
  else /* if(mode == CTR)*/
  {
    printf("CTR\n\r");
  }

  printf(" ---------------------------------------\n\r");

  for (BufferCounter = 0; BufferCounter < datalength; BufferCounter++)
  {
    printf("[0x%02X]", aDecryptedText[BufferCounter]);
    count++;

    if (count == 16)
    {
      count = 0;
      printf(" Block %lu \n\r", BufferCounter / 16);
    }
  }
}

/**
  * @brief  Wait till a character is received by the USART
  * @param  None
  * @retval None
  */
static void PressToContinue(void)
{
  uint8_t data = 0;

  printf("\n\r Press any key to continue...\n\r ");

  while (data == 0)
  {
    /* Read a character from the BSP_COM1 */
    HAL_UART_Receive(&UartHandle, (uint8_t *)&data, 1, TIMEOUT_VALUE);
  }
}

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the BSP_COM1 and Loop until the end of transmission */
  HAL_UART_Transmit(&UartHandle, (uint8_t *)&ch, 1, TIMEOUT_VALUE);

  return ch;
}
  
/**
  * @brief  Configures COM port.
  * @param  COM: Specifies the COM port to be configured.
  *   This parameter can be one of following parameters:    
  *     @arg COM1  
  * @param  huart: pointer to a UART_HandleTypeDef structure that
  *   contains the configuration information for the specified UART peripheral.
  * @retval None
  */
void BSP_COM_Init(COM_TypeDef COM, UART_HandleTypeDef* huart)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  /* Enable GPIO clock */
  COMx_TX_GPIO_CLK_ENABLE(COM);
  COMx_RX_GPIO_CLK_ENABLE(COM);

  /* Enable USART clock */
  COMx_CLK_ENABLE(COM);

  /* Configure USART Tx as alternate function push-pull */
  GPIO_InitStruct.Pin = COM_TX_PIN[COM];
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Alternate = COM_TX_AF[COM];
  HAL_GPIO_Init(COM_TX_PORT[COM], &GPIO_InitStruct);
    
  /* Configure USART Rx as alternate function push-pull */
  GPIO_InitStruct.Pin = COM_RX_PIN[COM];
  GPIO_InitStruct.Alternate = COM_RX_AF[COM];
  HAL_GPIO_Init(COM_RX_PORT[COM], &GPIO_InitStruct);

  /* USART configuration */
  huart->Instance = COM_USART[COM];
  HAL_UART_Init(huart);
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
