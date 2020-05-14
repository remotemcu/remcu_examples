/**
  ******************************************************************************
  * @file    RTC_Tamper/main.c 
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    20-September-2012
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
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

/** @addtogroup STM32F3_Discovery_Peripheral_Examples
  * @{
  */

/** @addtogroup RTC_Tamper
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define RTC_BKP_DR_NUMBER              10  /* RTC Backup Data Register Number */
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
NVIC_InitTypeDef NVIC_InitStructure;
EXTI_InitTypeDef  EXTI_InitStructure;
uint32_t RTC_BKP_DR[RTC_BKP_DR_NUMBER] =
{
  RTC_BKP_DR0, RTC_BKP_DR1, RTC_BKP_DR2, RTC_BKP_DR3, RTC_BKP_DR4,
  RTC_BKP_DR5, RTC_BKP_DR6, RTC_BKP_DR7, RTC_BKP_DR8, RTC_BKP_DR9
};
/* Private function prototypes -----------------------------------------------*/
static void RTC_BackUpDRWrite(uint32_t FirstRTCBackupData);
static uint32_t RTC_BackUpDRCheck(uint32_t FirstRTCBackupData);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief   Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f30x.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f30x.c file
     */     
  
  /* Initialize Leds mounted on STM32F3-discovery */
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED4);
  STM_EVAL_LEDInit(LED5);
  STM_EVAL_LEDInit(LED6);
   
  /* Enable the PWR clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

  /* Allow access to RTC */
  PWR_BackupAccessCmd(ENABLE);
  
  /* Reset Backup Domain */
  RCC_BackupResetCmd(ENABLE);
  RCC_BackupResetCmd(DISABLE);
  
  /* LSI used as RTC source clock*/
  /* The RTC Clock may varies due to LSI frequency dispersion. */
  /* Enable the LSI OSC */ 
  RCC_LSICmd(ENABLE);
  
  /* Wait till LSI is ready */  
  while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
  {
  }

  /* Select the RTC Clock Source */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
    
  /* Enable The external line19 interrupt */
  EXTI_ClearITPendingBit(EXTI_Line19);
  EXTI_InitStructure.EXTI_Line = EXTI_Line19;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  /* Enable RTC IRQChannel */
  NVIC_InitStructure.NVIC_IRQChannel = TAMPER_STAMP_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* Clear Tamper 2 pin Event(TAMP2F) pending flag */
  RTC_ClearFlag(RTC_FLAG_TAMP2F);

  /* Configure the Tamper 2 Trigger */
  RTC_TamperTriggerConfig(RTC_Tamper_2, RTC_TamperTrigger_FallingEdge);

  /* Enable the Tamper interrupt */
  RTC_ITConfig(RTC_IT_TAMP, ENABLE);

  /* Clear Tamper 2 pin interrupt pending bit */
  RTC_ClearITPendingBit(RTC_IT_TAMP2);

  /* Enable the Tamper 2 detection */
  RTC_TamperCmd(RTC_Tamper_2, ENABLE);
  
  /* Write To RTC Backup Data registers */
  RTC_BackUpDRWrite(0xA53C);

  /* Check if the written data are correct */
  if(RTC_BackUpDRCheck(0xA53C) == 0x00)
  {
    /* Turn on LED3 */
    STM_EVAL_LEDOn(LED3);
  }
  else
  {
    /* Turn on LED4 */
    STM_EVAL_LEDOn(LED4);
  }
  /* Infinite loop */
  while (1)
  {
  }
}

/**
  * @brief  Checks if the RTC Backup DRx registers are reset or not.
  * @param  None
  * @retval - 0: All RTC Backup DRx registers are reset
  *         - Value different from 0: Number of the first Backup register
  *           not reset
  */
uint32_t RTC_BackupRegResetCheck(void)
{
  uint32_t index = 0;

  for (index = 0; index < RTC_BKP_DR_NUMBER; index++)
  {
     /* Read from bkp Data Register */
    if (RTC_ReadBackupRegister(RTC_BKP_DR[index]) != 0x0)
    {
      return (index + 1);
    }
  }
  return 0;
}

/**
  * @brief  Checks if the RTC Backup DRx registers values are correct or not.
  * @param  FirstRTCBackupData: data to be compared with RTC Backup data registers.
  * @retval - 0: All RTC Backup DRx registers values are correct
  *         - Value different from 0: Number of the first Backup register
  *           which value is not correct
  */
static uint32_t RTC_BackUpDRCheck(uint32_t FirstRTCBackupData)
{
  uint32_t index = 0;

  for (index = 0; index < RTC_BKP_DR_NUMBER; index++)
  {
    /* Read from data register */
    if (RTC_ReadBackupRegister(RTC_BKP_DR[index]) != (FirstRTCBackupData + (index * 0x5A)))
    {
      return (index + 1);
    }
  }
    return 0;
}

/**
  * @brief  Writes data RTC Backup DRx registers.
  * @param  FirstRTCBackupData: data to be written to RTC Backup data registers.
  * @retval None
  */
static void RTC_BackUpDRWrite(uint32_t FirstRTCBackupData)
{
  uint32_t index = 0;

  for (index = 0; index < RTC_BKP_DR_NUMBER; index++)
  {
    /* write To bkp data register */
    RTC_WriteBackupRegister(RTC_BKP_DR[index], FirstRTCBackupData + (index * 0x5A));
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
