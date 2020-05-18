/**
  ******************************************************************************
  * @file    CRC/CRC_8BitsCRCMessage/main.c 
  * @author  MCD Application Team
  * @version V1.4.0
  * @date    24-July-2014
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
#include "main.h"

/** @addtogroup STM32F0xx_StdPeriph_Examples
  * @{
  */

/** @addtogroup CRC_8BitsCRCMessage
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define countof(a)             (uint8_t)(sizeof(a) / sizeof(*(a)))
#define BUFFER_SIZE            (countof(CRCBuffer) - 1)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t CRCBuffer[] = "STM32F0xx CortexM0 Device running on STM32072B-EVAL";
uint8_t ComputedCRC = 0;
uint8_t ExpectedCRC = 0xAF; /* The expected CRC value of CRCBuffer using the
                               polynomial x8 + x7 + x6 + x4 + x2 + 1.
                               This value is already computed using on-line CRC tool */

/* Private function prototypes -----------------------------------------------*/
static void CRC_Config(uint8_t poly);
static uint8_t CRC_8BitsCompute(uint8_t* data, uint32_t size);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f0xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f0xx.c file
     */

  /* Initialize LEDs available on STM32072B-EVAL board */
  STM_EVAL_LEDInit(LED1);
  STM_EVAL_LEDInit(LED3);

  /* Configure the CRC peripheral to use the polynomial x8 + x7 + x6 + x4 + x2 + 1 */
  CRC_Config(0xD5);

  /* Compute the CRC value of the 8-bit buffer: CRCBuffer */
  ComputedCRC = CRC_8BitsCompute(CRCBuffer, BUFFER_SIZE);

  /* Check if the computed CRC matches the expected one */
  if (ComputedCRC != ExpectedCRC)
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
  while(1)
  {
  }
}

/**
  * @brief  Configure CRC peripheral to use 8-bit polynomials
  * @param  poly: the CRC polynomial
  * @retval None
  */
static void CRC_Config(uint8_t poly)
{
  /* Enable CRC AHB clock interface */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);

  /* DeInit CRC peripheral */
  CRC_DeInit();
  
  /* Init the INIT register */
  CRC_SetInitRegister(0);
  
  /* Select 8-bit polynomial size */
  CRC_PolynomialSizeSelect(CRC_PolSize_8);
  
  /* Set the polynomial coefficients */
  CRC_SetPolynomial(poly);
}

/**
  * @brief  Compute CRC value for input message
  * @param  data: pointer at uint8_t
  * @param  size: the size of the input message
  * @retval The computed CRC value
  */
static uint8_t CRC_8BitsCompute(uint8_t* data, uint32_t size)
{
  uint8_t* dataEnd = data+size;

  /* Reset CRC data register to avoid overlap when computing new data stream */
  CRC_ResetDR();

  while(data < dataEnd)
  {
    /* Write the input data in the CRC data register */
    CRC_CalcCRC8bits(*data++);
  }
  /* Return the CRC value */
  return (uint8_t)CRC_GetCRC();
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
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
