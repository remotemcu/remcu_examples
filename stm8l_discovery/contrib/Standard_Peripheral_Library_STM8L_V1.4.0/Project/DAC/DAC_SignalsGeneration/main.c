/**
  ******************************************************************************
  * @file    DAC/DAC_SignalsGeneration/main.c
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
#include "main.h"

/** @addtogroup STM8L15x_StdPeriph_Examples
  * @{
  */

/** @addtogroup DAC_SignalsGeneration
  * @{
  */


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
CONST uint16_t SinCard12bit[130] = {892, 886, 864, 828, 780, 726, 670, 617, 574, 545, 532,
                                      539, 565, 608, 664, 729, 797, 860, 913, 950, 966, 959,
                                      928, 876, 807, 726, 641, 560, 492, 444, 421, 428, 466,
                                      532, 622, 729, 843, 954, 1050, 1120, 1156, 1152, 1104,
                                      1013, 884, 726, 551, 376, 216, 90, 13, 0, 61, 204, 428,
                                      729, 1097, 1517, 1968, 2428, 2872, 3276, 3618, 3877,
                                      4039, 4094, 4039, 3877, 3618, 3276, 2872, 2428, 1968,
                                      1517, 1097, 729, 428, 204, 61, 0, 13, 90, 216, 376, 551,
                                      726, 884, 1013, 1104, 1152, 1156, 1120, 1050, 954, 843,
                                      729, 622, 532, 466, 428, 421, 444, 492, 560, 641, 726,
                                      807, 876, 928, 959, 966, 950, 913, 860, 797, 729, 664,
                                      608, 565, 539, 532, 545, 574, 617, 670, 726, 780, 828
                                      , 864, 886 };

CONST uint16_t Triang12bit[130] = {0, 60, 120, 180, 240, 300, 360, 420, 480, 540, 600, 660,
                                     720, 780, 840, 900, 960, 1020, 1080, 1140, 1200, 1260,
                                     1320, 1380, 1440, 1500, 1560, 1620, 1680, 1740, 1800,
                                     1860, 1920, 1980, 2040, 2100, 2160, 2220, 2280, 2340,
                                     2400, 2460, 2520, 2580, 2640, 2700, 2760, 2820, 2880,
                                     2940, 3000, 3060, 3120, 3180, 3240, 3300, 3360, 3420,
                                     3480, 3540, 3600, 3660, 3720, 3780, 3840, 3900, 3840,
                                     3780, 3720, 3660, 3600, 3540, 3480, 3420, 3360, 3300,
                                     3240, 3180, 3120, 3060, 3000, 2940, 2880, 2820, 2760,
                                     2700, 2640, 2580, 2520, 2460, 2400, 2340, 2280, 2220,
                                     2160, 2100, 2040, 1980, 1920, 1860, 1800, 1740, 1680,
                                     1620, 1560, 1500, 1440, 1380, 1320, 1260, 1200, 1140,
                                     1080, 1020, 960, 900, 840, 780, 720, 660, 600, 540, 480,
                                     420, 360, 300, 240, 180, 120, 60};

CONST uint16_t Sinus12bit[130] = {1800, 1887, 1974, 2060, 2146, 2231, 2315, 2397, 2479,
                                    2559, 2637, 2713, 2786, 2858, 2927, 2994, 3057, 3118,
                                    3176, 3230, 3281, 3329, 3373, 3414, 3450, 3483, 3512,
                                    3537, 3558, 3574, 3587, 3595, 3599, 3599, 3595, 3587,
                                    3574, 3558, 3537, 3512, 3483, 3450, 3414, 3373, 3329,
                                    3281, 3230, 3176, 3118, 3057, 2994, 2927, 2858, 2786,
                                    2713, 2637, 2559, 2479, 2397, 2315, 2231, 2146, 2060,
                                    1974, 1887, 1800, 1713, 1626, 1540, 1454, 1369, 1285,
                                    1203, 1121, 1041, 963, 887, 814, 742, 673, 606, 543, 482,
                                    424, 370, 319, 271, 227, 186, 150, 117, 88, 63, 42, 26, 13,
                                    5, 1, 1, 5, 13, 26, 42, 63, 88, 117, 150, 186, 227, 271, 319,
                                    370, 424, 482, 543, 606, 673, 742, 814, 887, 963, 1041,
                                    1121, 1203, 1285, 1369, 1454, 1540, 1626, 1713};

CONST uint16_t Square12bit[130] = {4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095,
                                     4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095,
                                     4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095,
                                     4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095,
                                     4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095,
                                     4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095,
                                     4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095,
                                     4095, 4095, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

CONST uint16_t WavesTab[WAVES_TAB_SIZE] = {(uint16_t)&Triang12bit,
    (uint16_t)&Square12bit,
    (uint16_t)&SinCard12bit,
    (uint16_t)&Sinus12bit};


/* Private function prototypes -----------------------------------------------*/
static void CLK_Config(void);
static void DMA_Config(void);
static void DAC_Config(void);
static void TIM4_Config(void);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
void main(void)
{
   /* CLK configuration -------------------------------------------*/
  CLK_Config(); 
  
   /* DMA configuration -------------------------------------------*/
  DMA_Config(); 

   /* DAC configuration -------------------------------------------*/
  DAC_Config(); 
  
   /* TIM4 configuration -------------------------------------------*/
  TIM4_Config(); 


  /* Init Eval Board used Buttons*/
  STM_EVAL_PBInit(BUTTON_RIGHT, BUTTON_MODE_EXTI);
  STM_EVAL_PBInit(BUTTON_LEFT, BUTTON_MODE_EXTI);

  enableInterrupts();

  /* Infinite loop */
  while (1)
  {}
}

/**
  * @brief  Configure peripheral clock 
  * @param  None
  * @retval None
  */
static void CLK_Config(void)
{
  /* Enable Clocks of DAC, TIM4 and DMA1 */
  CLK_PeripheralClockConfig(CLK_Peripheral_DAC, ENABLE);
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM4, ENABLE);
  CLK_PeripheralClockConfig(CLK_Peripheral_DMA1, ENABLE);

}

/**
  * @brief  Configure DMA peripheral 
  * @param  None
  * @retval None
  */
static void DMA_Config(void)
{
  /* DMA channel3 Config -----------------------------------------------------*/
  DMA_GlobalDeInit();
  DMA_DeInit(DMA1_Channel3);
  DMA_Init(DMA1_Channel3,  WavesTab[0], DAC_CH1RDHRH_ADDRESS,
           130, DMA_DIR_MemoryToPeripheral, DMA_Mode_Circular,
           DMA_MemoryIncMode_Inc, DMA_Priority_High, DMA_MemoryDataSize_HalfWord);

  /* DMA1 Channel 3 enable */
  DMA_Cmd(DMA1_Channel3, ENABLE);
  DMA_GlobalCmd(ENABLE);
}

/**
  * @brief  Configure DAC peripheral 
  * @param  None
  * @retval None
  */
static void DAC_Config(void)
{
  /* DAC Channel1 Config: 12bit right ----------------------------------------*/
  /* DAC deinitialize */
  DAC_DeInit();
  
  /* Fill DAC Init param DAC_Trigger_T4_TRGO and  DAC Channel1 Init */
  DAC_Init(DAC_Channel_1, DAC_Trigger_T4_TRGO, DAC_OutputBuffer_Enable);

  /* Enable DAC Channel1 */
  DAC_Cmd(DAC_Channel_1, ENABLE);
  
  /* Enable DMA for DAC Channel1 */
  DAC_DMACmd(DAC_Channel_1, ENABLE);
}

/**
  * @brief  Configure TIM4 peripheral 
  * @param  None
  * @retval None
  */
static void TIM4_Config(void)
{
  TIM4_DeInit();
  /* Time base configuration */
  TIM4_TimeBaseInit(TIM4_Prescaler_1, 0x0F);
  
  /* TIM4 TRGO selection */
  TIM4_SelectOutputTrigger(TIM4_TRGOSource_Update);
  
  /* TIM4 enable counter */
  TIM4_Cmd(ENABLE);
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
