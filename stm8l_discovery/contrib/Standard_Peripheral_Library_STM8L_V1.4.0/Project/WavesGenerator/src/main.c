/**
  ******************************************************************************
  * @file    DAC/DAC_SignalsGeneration/main.c
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    02/12/2010
  * @brief   Main program body
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

/* Includes ------------------------------------------------------------------*/
#include "stm8l15x.h"
#include "stm8l_discovery_lcd.h"
#include "main.h"
#include "stm8l15x_lcd.h"


/* Define to prevent recursive inclusion -------------------------------------*/

/** @addtogroup STM8L15x_StdPeriph_Examples
  * @{
  */

/** @addtogroup DAC_SignalsGeneration
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define SCROLL_SPEED 400  /* Low value gives higher speed */
#define SCROLL_SPEED2 300 /* Low value gives higher speed */
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__CONST uint16_t SinCard12bit[130] = {892, 886, 864, 828, 780, 726, 670, 617, 574, 545, 532,
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

__CONST uint16_t Triang12bit[130] = {0, 60, 120, 180, 240, 300, 360, 420, 480, 540, 600, 660,
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

__CONST uint16_t Sinus12bit[130] = {1800, 1887, 1974, 2060, 2146, 2231, 2315, 2397, 2479,
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

__CONST uint16_t Square12bit[130] = {4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095,
                                     4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095,
                                     4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095,
                                     4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095,
                                     4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095,
                                     4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095,
                                     4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095,
                                     4095, 4095, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
																		 
																		 
__CONST uint16_t SawTooth12bit[130] = {0, 31, 63, 95, 126, 158, 190, 222, 253, 285, 317, 349,
																		 380, 412, 444, 476, 507, 539, 571, 603, 634, 666, 698,
																		 730, 761, 793, 825, 857, 888, 920, 952, 984, 1015, 1047,
																		 1079, 1111, 1142, 1174, 1206, 1238, 1269, 1301, 1333,
																		 1365, 1396, 1428, 1460, 1491, 1523, 1555, 1587, 1618,
																		 1650, 1682, 1714, 1745, 1777, 1809, 1841, 1872, 1904,
																		 1936, 1968, 1999, 2031, 2063, 2095, 2126, 2158, 2190,
																		 2222, 2253, 2285, 2317, 2349, 2380, 2412, 2444, 2476,
																		 2507, 2539, 2571, 2603, 2634, 2666, 2698, 2730, 2761,
																		 2793, 2825, 2856, 2888, 2920, 2952, 2983, 3015, 3047,
																		 3079, 3110, 3142, 3174, 3206, 3237, 3269, 3301, 3333,
																		 3364, 3396, 3428, 3460, 3491, 3523, 3555, 3587, 3618,
																		 3650, 3682, 3714, 3745, 3777, 3809, 3841, 3872, 3904,
																		 3936, 3968, 3999, 4031, 4063, 4095};
																		 
__CONST uint16_t SinCard12bit2[65] = {1032, 1056, 1019, 931, 825, 741, 715, 762, 868, 996, 1096, 1125, 
																		 1065, 931, 771, 645, 610, 690, 868, 1084, 1257, 1307, 1194, 
																		 931, 597, 312, 207, 381, 868, 1613, 2485, 3302, 3883, 4095, 
																		 3883, 3302, 2485, 1613, 868, 381, 207, 312, 597, 931, 1194, 
																		 1307, 1257, 1084, 868, 690, 610, 645, 771, 931, 1065, 1125,
																		 1096, 996, 868, 762, 715, 741, 825, 931, 1019};
																		 
__CONST uint16_t Triang12bit2[65] = {0, 120, 240, 360, 480, 600, 720, 840, 960, 1080, 1200, 
                                     1320, 1440, 1560, 1680, 1800, 1920, 2040, 2160, 2280, 2400,
																		 2520,  2640,  2760,  2880, 3000, 3120, 3240, 3360, 
                                     3480, 3600, 3720, 3840, 3720, 3600, 3480, 3360, 3240, 3120, 
																		 3000, 2880, 2760, 2640, 2520, 2400, 2280, 2160, 2040, 1920, 
																		 1800, 1680, 1560, 1440, 1320, 1200, 1080, 960, 840, 720, 600,
																		 480, 360, 240, 120, 60};
																		 
__CONST uint16_t Sinus12bit2[65] = {2047, 2241, 2431, 2619, 2801, 2977, 3143, 3299, 3445,
																		3575, 3693, 3795, 3881, 3949, 3999, 4033, 4047, 4041, 
																		4019, 3977, 3917, 3839, 3745, 3637, 3511, 3373, 3223, 
																		3061, 2889, 2711, 2525, 2335, 2143, 1951, 1759, 1569, 
																		1383, 1205, 1033, 871, 721, 583, 457, 349, 255, 177, 
																		117, 75, 53, 47, 61, 95, 145, 213, 299, 401, 519, 649, 
																		795, 951, 1117, 1293, 1475, 1663, 1853};

__CONST uint16_t Square12bit2[65] = {4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095,
                                     4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095,
                                     4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095,
                                     4095, 4095, 4095, 4095, 4095,0, 0, 0, 0, 0, 0, 0, 0,
																		 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
																		 0, 0, 0, 0, 0, 0};

__CONST uint16_t SawTooth12bit2[65] = {0, 63, 126, 190, 253, 317, 380, 444, 507, 571, 634,
																			698, 761, 825, 888, 952, 1015, 1079, 1142, 1206, 1269,
																			1333, 1396, 1460, 1523, 1587, 1650, 1714, 1777, 1841,
																			1904, 1968, 2031, 2095, 2158, 2222, 2285, 2349, 2412,
																			2476, 2539, 2603, 2666, 2730, 2793, 2856, 2920, 2983,
																			3047, 3110, 3174, 3237, 3301, 3364, 3428, 3491, 3555,
																			3618, 3682, 3745, 3809, 3872, 3936, 3999, 4063};
																		 
__CONST uint16_t WavesTab[WAVES_TAB_SIZE] = {(uint16_t)&Triang12bit,
                                             (uint16_t)&Square12bit,
                                             (uint16_t)&SinCard12bit,
                                             (uint16_t)&Sinus12bit,
																						 (uint16_t)&SawTooth12bit,
																						 (uint16_t)&Triang12bit2,
																						 (uint16_t)&Square12bit2,
																						 (uint16_t)&SinCard12bit2,
																						 (uint16_t)&Sinus12bit2,
																						 (uint16_t)&SawTooth12bit2};
/* Global variables ---------------------------------------------------------*/
uint8_t wave_type = 0;
uint8_t display_loop = 0;
uint8_t sample_size;
/* tab_index variable refers to the first element of WavesTab(ramp) */
uint8_t tab_index =0;
bool priority = TRUE;
extern uint8_t t_bar[2];
/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
void main(void)
{
	/* variables */
	uint8_t tmpreg1 = 0;
	sample_size = 130;
	/* Set System Clock divider to 1 */
	CLK->CKDIVR = 0x00;
  /* Enable Clocks of DAC, TIM4, DMA1 */
	/* DAC */
	CLK->PCKENR1 |= 0x80;
	/* TIM4 */
	CLK->PCKENR1 |= 0x04;
	/* DMA1 */
	CLK->PCKENR2 |= 0x10;
	/* DMA Init */					
  /* Disable the selected DMA Channelx */
  DMA1_Channel3->CCR &= (uint8_t)~(DMA_CCR_CE);
  /* Reset DMA Channelx control register */
  DMA1_Channel3->CCR  = DMA_CCR_RESET_VALUE;
  /* Set DMA direction & Mode & Incremantal Memory mode */
  DMA1_Channel3->CCR |= ((uint8_t)DMA_DIR_MemoryToPeripheral | (uint8_t)DMA_Mode_Circular |(uint8_t) DMA_MemoryIncMode_Inc);
  /*Clear old priority and memory data size  option */
  DMA1_Channel3->CSPR &= (uint8_t)~(DMA_CSPR_PL | DMA_CSPR_16BM);
  /* Set old priority and memory data size  option */
  DMA1_Channel3->CSPR |= ((uint8_t)DMA_Priority_VeryHigh |(uint8_t)DMA_MemoryDataSize_HalfWord);
  /* Write to DMA Channelx CNBTR */
  DMA1_Channel3->CNBTR = (uint8_t)sample_size;
  /*--------------------------- DMA Channel3 CPAR Configuration ----------------*/
  DMA1_Channel3->CPARH = (uint8_t)(DAC_CH1RDHRH_ADDRESS >> (uint8_t)8);
  DMA1_Channel3->CPARL = (uint8_t)(DAC_CH1RDHRH_ADDRESS);
  /*--------------------------- DMA Channel3 CMAR Configuration ----------------*/
  DMA1_Channel3->CM0ARH = (uint8_t)(WavesTab[tab_index] >> (uint8_t)8);
  DMA1_Channel3->CM0ARL = (uint8_t)(WavesTab[tab_index]);	
  /* DMA1 Channel 3 enable */
	DMA1_Channel3->CCR |= DMA_CCR_CE;
  /* Enable the  DMA      */
  DMA1->GCSR |= (uint8_t)DMA_GCSR_GE;

  /* DAC Channel1 Config: 12bit right-----------------------------------------------------*/
  /* Fill DAC Init param DAC_Trigger_T4_TRGO* and  DAC Channel1 Init */
	/* DAC CR1 Config */
  tmpreg1 = DAC->CH1CR1;
  /* Clear TENx, TSELx bits */
	tmpreg1 &= (uint8_t)~(DAC_CR1_TEN | DAC_CR1_TSEL );
  /* Configure for the selected DAC channel: buffer output, trigger*/
  /* Set BOFFx bit Output Buffer disable */
  tmpreg1 |= (uint8_t)(DAC_CR1_BOFF );
	/* Set TSELx and TEN  bits according to DAC_Trigger value */
	tmpreg1 |= (uint8_t)(DAC_CR1_TEN | DAC_Trigger_T4_TRGO) ;
  /* Write to DAC CR1 */
  DAC->CH1CR1 = tmpreg1;
  /* Enable DAC Channel1 */
	DAC->CH1CR1 |= DAC_CR1_EN;
	DAC->CH1CR2 |= DAC_CR2_DMAEN;
	
  /********* TIM4 Config ********/
	/* Set output signal frequency approx. equal to 100Hz Range 1 */
  TIM4->ARR = (uint8_t)(0x4C);
  /* Set the Prescaler value */
  TIM4->PSCR = (uint8_t)(TIM4_Prescaler_16);
	
  /* TIM4 TRGO selection */
  tmpreg1 = TIM4->CR2;
  /* Reset the MMS Bits */
  tmpreg1 &= (uint8_t)(~TIM4_CR2_MMS);
  /* Select the TRGO source */
  tmpreg1 |=  (uint8_t)TIM4_TRGOSource_Update;
  TIM4->CR2 = tmpreg1;
	
  /* TIM4 enable counter */
	TIM4->CR1 |= TIM4_CR1_CEN ;
	
  /* Init Board key button and led */
	/* Interrupt sensitivity set to falling edge only for PC1 */
	EXTI->CR1 = 0x0A;
	/* Init GPIO Port C PC1 pin input floating with interrupt */
	GPIOC->DDR &= 0xFD;		
	GPIOC->CR1 &= 0xFD;	
	GPIOC->CR2 |= 0x02;
	/* Init GPIO Port C PC7 output push-pull up to 10MHz blue LED4*/
	GPIOC->DDR |= 0x80;		
	GPIOC->CR1 |= 0x80;	
	GPIOC->CR2 |= 0x80;
	/* Init GPIO Port E PE7 output push-pull up to 10MHz green LED3*/
	GPIOE->DDR |= 0x80;		
	GPIOE->CR1 |= 0x80;	
	GPIOE->CR2 |= 0x80;


	/* Initialization of LCD */
	LCD_GLASS_Init();
	/* Set standard contrast */
	LCD_ContrastConfig(LCD_Contrast_3V0);
	/* Display bars */
	BAR0_ON;
	BAR1_OFF;
	BAR3_OFF;


	enableInterrupts();
  /* Infinite loop */
  while (1)
	{	
	
		/* Display entry message */
		while(priority)
			LCD_GLASS_ScrollSentence("* * * STM8L DISCOVERY WAVE GENERATOR * * * ", 1, SCROLL_SPEED);
		switch (wave_type)
		{
			case 0:
					LCD_GLASS_ScrollSentence("RAMP * * ",1,SCROLL_SPEED2);
					display_loop++;
					break;
			case 1:
					LCD_GLASS_ScrollSentence("SQUARE * * ",1,SCROLL_SPEED2);
					display_loop++;
					break;
			case 2: 
					LCD_GLASS_ScrollSentence("SINC * * ",1,SCROLL_SPEED2);
					display_loop++;
					break;
			case 3:
					LCD_GLASS_ScrollSentence("SINE * * ",1,SCROLL_SPEED2);
					display_loop++;
					break;
			case 4: 
					LCD_GLASS_ScrollSentence("SAWTOOTH * * ",1,SCROLL_SPEED2);
					display_loop++;
					break;
		}
		/* If no activity during 10 display times then allows entry message */
		if (display_loop>10)
			priority = TRUE;

	}
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
/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
