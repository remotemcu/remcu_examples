/**
  ******************************************************************************
  * @file    AES/AES_KeyDerivation/main.c
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

/* Includes ------------------------------------------------------------------*/
#include "stm8l15x.h"
#include "stm8_eval.h"

/** @addtogroup STM8L15x_StdPeriph_Examples
  * @{
  */

/** @addtogroup AES_KeyDerivation
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t EncryptionKey[16] = "ultra-low power.";
uint8_t DecryptionKey[16];
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
void main(void)
{
  uint8_t arrayindex = 0;

  /* Enable AES clock */
  CLK_PeripheralClockConfig(CLK_Peripheral_AES, ENABLE);

  /* Initialize LEDs mounted on STM8L1528-EVAL board */
  STM_EVAL_LEDInit(LED1);
  STM_EVAL_LEDInit(LED3);

  /* Select the key derivation mode */
  AES_OperationModeConfig(AES_Operation_KeyDeriv);

  /* Enable the AES peripheral */
  AES_Cmd(ENABLE);

  /* write 16 times in the DINR register with encryption key */
  while (arrayindex < 16)
  {
    /* writing MSB first */
    AES_WriteSubKey(EncryptionKey[arrayindex]);
    /* Increment arrayindex */
    arrayindex++;
  }

  /* Wait for CCF flag to be set */
  while (AES_GetFlagStatus(AES_FLAG_CCF) == RESET);

  /* Clear CCF flag */
  AES_ClearFlag(AES_FLAG_CCF);

  /* Init arrayindex */
  arrayindex = 0;

  /* read 16 times the DOUTR register to get the decryption key */
  while (arrayindex < 16)
  {
    /* Reading MSB first */
    DecryptionKey[arrayindex] = AES_ReadSubData();
    /* Increment arrayindex */
    arrayindex++;
  }

  /* Check errors flags */
  if ((AES_GetFlagStatus(AES_FLAG_RDERR) != RESET) || (AES_GetFlagStatus(AES_FLAG_WRERR) != RESET))
  {
    /* Turn on LD3 */
    STM_EVAL_LEDOn(LED3);
  }
  else
  {
    /* Turn on LD1 */
    STM_EVAL_LEDOn(LED1);
  }

  /* Infinite loop */
  while (1)
  {}
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {}
}
#endif

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
