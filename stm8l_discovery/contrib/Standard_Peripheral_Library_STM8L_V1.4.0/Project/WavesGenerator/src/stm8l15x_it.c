/**
  ******************************************************************************
  * @file    Project/Template/stm8l15x_it.c
  * @author  MCD Tools Team
  * @version V1.1.0
  * @date    08/2010
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all peripherals interrupt service routine.
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
#include "stm8l15x_it.h"
#include "main.h"
#include "stm8l_discovery_lcd.h"
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


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern __CONST uint16_t WavesTab[];
__IO uint8_t  TIM4PresTabIndex = 1;
uint8_t  WavesTabIndex = 1;
	extern uint8_t t_bar[2];
	extern uint8_t wave_type;
	extern bool priority;
	extern uint8_t display_loop;
	extern uint8_t sample_size;   
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/

/** @addtogroup IT_Functions
  * @{
  */
INTERRUPT_HANDLER(NonHandledInterrupt,0)
{
/* In order to detect unexpected events during development,
   it is recommended to set a breakpoint on the following instruction.
*/
  while (1);

}
/**
  * @brief TRAP interrupt routine
  * @par Parameters:
  * None
  * @retval 
  * None
*/
INTERRUPT_HANDLER_TRAP(TRAP_IRQHandler)
{
/* In order to detect unexpected events during development,
   it is recommended to set a breakpoint on the following instruction.
*/
  while (1);

}
/**
  * @brief FLASH Interrupt routine.
  * @par Parameters:
  * None
  * @retval 
  * None
  */
INTERRUPT_HANDLER(FLASH_IRQHandler,1)
{
/* In order to detect unexpected events during development,
   it is recommended to set a breakpoint on the following instruction.
*/
  while (1);

}
/**
  * @brief DMA1 channel0 and channel1 Interrupt routine.
  * @par Parameters:
  * None
  * @retval 
  * None
  */
INTERRUPT_HANDLER(DMA1_CHANNEL0_1_IRQHandler,2)
{
/* In order to detect unexpected events during development,
   it is recommended to set a breakpoint on the following instruction.
*/
  while (1);

}
/**
  * @brief DMA1 channel2 and channel3 Interrupt routine.
  * @par Parameters:
  * None
  * @retval 
  * None
  */
INTERRUPT_HANDLER(DMA1_CHANNEL2_3_IRQHandler,3)
{
/* In order to detect unexpected events during development,
   it is recommended to set a breakpoint on the following instruction.
*/
  while (1);

}
/**
  * @brief RTC Interrupt routine.
  * @par Parameters:
  * None
  * @retval 
  * None
  */

INTERRUPT_HANDLER(RTC_IRQHandler,4)
{
/* In order to detect unexpected events during development,
   it is recommended to set a breakpoint on the following instruction.
*/
  while (1);

}
/**
  * @brief External IT PORTE/F and PVD Interrupt routine.
  * @par Parameters:
  * None
  * @retval 
  * None
  */
INTERRUPT_HANDLER(EXTIE_F_PVD_IRQHandler,5)
{
/* In order to detect unexpected events during development,
   it is recommended to set a breakpoint on the following instruction.
*/
  while (1);

}

/**
  * @brief External IT PORTB Interrupt routine.
  * @par Parameters:
  * None
  * @retval 
  * None
  */
INTERRUPT_HANDLER(EXTIB_IRQHandler,6)
{
/* In order to detect unexpected events during development,
   it is recommended to set a breakpoint on the following instruction.
*/
  while (1);

}

/**
  * @brief External IT PORTD Interrupt routine.
  * @par Parameters:
  * None
  * @retval 
  * None
  */
INTERRUPT_HANDLER(EXTID_IRQHandler,7)
{
/* In order to detect unexpected events during development,
   it is recommended to set a breakpoint on the following instruction.
*/
  while (1);

}

/**
  * @brief External IT PIN0 Interrupt routine.
  * @par Parameters:
  * None
  * @retval 
  * None
  */

INTERRUPT_HANDLER(EXTI0_IRQHandler,8)
{
/* In order to detect unexpected events during development,
   it is recommended to set a breakpoint on the following instruction.
*/
  while (1);

}
/**
  * @brief External IT PIN1 Interrupt routine.
  *   On User button pressed:
  *   Check if button presse a long time (4-5 sec.) if yes --> Set Autotest
  *   Else update status_machine to pass to next measuremetn.
  *   Update the LCD bar graph
  * @par Parameters:
  * None
  * @retval 
  * None
  */
INTERRUPT_HANDLER(EXTI1_IRQHandler,9)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
// IT Linked to USER button present on board on PC1 detected on falling edge only
	if (!(GPIOC->IDR & 0x02))
  {
			__IO uint32_t tmp = 100000, i;
		   uint8_t speed = 0;
			/* Detect USER button key pressed for more than 250 ms: TUNE FREQ MODE */
			while (!(GPIOC->IDR & 0x02) && tmp !=0)
				tmp-- ;
			if (tmp == 0)
			{	
				while (!(GPIOC->IDR & 0x02))
				{	
					/* Increase change speed */
					speed++;
					if (speed>50) 
						speed=50;
					/* Decrement TIM4 Auto reload value to change DAC freq signal */
					(TIM4->ARR)--;
					/* Check timer low value to avoid DAC overrrun */
					if (TIM4->ARR <= 0x09)
					{
						TIM4->ARR = 0xFF;
						/* Change TIM4 prescaler according current range */
						if (TIM4->PSCR == 0x00)
						{
							TIM4->PSCR = 0x04;
							BAR1_OFF;
						}
						else
						{
							TIM4->PSCR = 0x00;
							BAR1_ON;
						}
					}
					/* tempo between 2 freq changes anf blue LED4 ON */
					GPIOC->ODR |= 0x80;
					LCD_GLASS_DisplayString("TUNING");
					for(i=0;i<(100000/speed);i++);
					GPIOC->ODR &= 0x7F;
				}
			}
			else
			{ 
				/* If USER button key pressed one single time
				then change wave shapes among waves table: CHANGE WAVE MODE */			
				/* Init display */
				display_loop = 0;
				/* Green LED2 ON */
				GPIOE->ODR |= 0x80;
				/* Change waveform by re-initializing DMA data and addresses */
				DMA1->GCSR &= (uint8_t)(~DMA_GCSR_GE);
				/* Disable the selected DMA Channelx */
				DMA1_Channel3->CCR &= (uint8_t)(~DMA_CCR_CE);
				DMA1_Channel3->CCR &= (uint8_t)(~DMA_CCR_CE);
				/* Reset DMA Channelx control register */
				DMA1_Channel3->CCR  = DMA_CCR_RESET_VALUE;
				/* Set DMA direction & Mode & Incremantal Memory mode */
				DMA1_Channel3->CCR |= ((uint8_t)DMA_DIR_MemoryToPeripheral |(uint8_t) DMA_Mode_Circular |(uint8_t)DMA_MemoryIncMode_Inc);
				/*Clear old priority and memory data size  option */
				DMA1_Channel3->CSPR &= (uint8_t)~(DMA_CSPR_PL | DMA_CSPR_16BM);
				/* Set old priority and memory data size  option */
				DMA1_Channel3->CSPR |= ((uint8_t)DMA_Priority_VeryHigh |(uint8_t)DMA_MemoryDataSize_HalfWord);
				/*--------------------------- DMA Channelx CNDTR Configuration ---------------*/
				/* Write to DMA Channelx CNDTR */
				DMA1_Channel3->CNBTR = (uint8_t)sample_size;
				/*--------------------------- DMA Channelx CPAR Configuration ----------------*/
				/* Write to DMA Channelx (0, 1 or 2)  Peripheral address  */
				/* Write to DMA Channel 3 Memory 1 address  */
				DMA1_Channel3->CPARH = (uint8_t)(DAC_CH1RDHRH_ADDRESS >> (uint8_t)8);
				DMA1_Channel3->CPARL = (uint8_t)(DAC_CH1RDHRH_ADDRESS);
				/*--------------------------- DMA Channelx CMAR Configuration ----------------*/
				/* Write to DMA Channelx Memory address */
				DMA1_Channel3->CM0ARH = (uint8_t)(WavesTab[WavesTabIndex] >> (uint8_t)8);
				DMA1_Channel3->CM0ARL = (uint8_t)(WavesTab[WavesTabIndex]);	
				/* DMA1 Channel 3 enable */
				DMA1_Channel3->CCR |= DMA_CCR_CE;
				/* Enable the  DMA      */
				DMA1->GCSR |= (uint8_t)DMA_GCSR_GE;
				/* Increment Wave Tab Index compare to Tab size and corresponding wave_type  */
				WavesTabIndex++;	
				if (WavesTabIndex == WAVES_TAB_SIZE)
				{
					WavesTabIndex = 0;
					sample_size = 130;
					BAR3_OFF;
				}
				if (WavesTabIndex > 4)
				{
					sample_size = 65;
					BAR3_ON;
				}
				wave_type++;	
				if (wave_type>4)
					wave_type = 0;
				/* delay to see LED change */
				for(i=0;i<(100000);i++);
				/* Green LED OFF */
				GPIOE->ODR &= 0x7F;
			}
		/* Clear interrupt flag */
		EXTI->SR1 = (uint8_t)0x02;
		/* Disable intro message STM8L DISCOVERY */
		priority = FALSE;
  }


}

/**
  * @brief External IT PIN2 Interrupt routine.
  * @par Parameters:
  * None
  * @retval 
  * None
  */

INTERRUPT_HANDLER(EXTI2_IRQHandler,10)
{
/* In order to detect unexpected events during development,
   it is recommended to set a breakpoint on the following instruction.
*/
  while (1);

}

/**
  * @brief External IT PIN3 Interrupt routine.
  * @par Parameters:
  * None
  * @retval 
  * None
  */
INTERRUPT_HANDLER(EXTI3_IRQHandler,11)
{
/* In order to detect unexpected events during development,
   it is recommended to set a breakpoint on the following instruction.
*/
  while (1);

}

/**
  * @brief External IT PIN4 Interrupt routine.
  * @par Parameters:
  * None
  * @retval 
  * None
  */
INTERRUPT_HANDLER(EXTI4_IRQHandler,12)
{
/* In order to detect unexpected events during development,
   it is recommended to set a breakpoint on the following instruction.
*/
  while (1);

}

/**
  * @brief External IT PIN5 Interrupt routine.
  * @par Parameters:
  * None
  * @retval 
  * None
  */
INTERRUPT_HANDLER(EXTI5_IRQHandler,13)
{
/* In order to detect unexpected events during development,
   it is recommended to set a breakpoint on the following instruction.
*/
  while (1);
  
}

/**
  * @brief External IT PIN6 Interrupt routine.
  * @par Parameters:
  * None
  * @retval 
  * None
  */
INTERRUPT_HANDLER(EXTI6_IRQHandler,14)
{
 
}

/**
  * @brief External IT PIN7 Interrupt routine.
  * @par Parameters:
  * None
  * @retval 
  * None
  */
INTERRUPT_HANDLER(EXTI7_IRQHandler,15)
{
/* In order to detect unexpected events during development,
   it is recommended to set a breakpoint on the following instruction.
*/
  
  while (1);

}
/**
  * @brief LCD start of new frame Interrupt routine.
  * @par Parameters:
  * None
  * @retval 
  * None
  */
INTERRUPT_HANDLER(LCD_IRQHandler,16)
{
/* In order to detect unexpected events during development,
   it is recommended to set a breakpoint on the following instruction.
*/
  while (1);

}
/**
  * @brief CLK switch/CSS/TIM1 break Interrupt routine.
  * @par Parameters:
  * None
  * @retval 
  * None
  */
INTERRUPT_HANDLER(SWITCH_CSS_BREAK_DAC_IRQHandler,17)
{
/* In order to detect unexpected events during development,
   it is recommended to set a breakpoint on the following instruction.
*/
  while (1);

}

/**
  * @brief ADC1/Comparator Interrupt routine.
  * @par Parameters:
  * None
  * @retval 
  * None
  */
INTERRUPT_HANDLER(ADC1_COMP_IRQHandler,18)
{
/* In order to detect unexpected events during development,
   it is recommended to set a breakpoint on the following instruction.
*/
  while (1);

}

/**
  * @brief TIM2 Update/Overflow/Trigger/Break Interrupt routine.
  * @par Parameters:
  * None
  * @retval 
  * None
  */
INTERRUPT_HANDLER(TIM2_UPD_OVF_TRG_BRK_IRQHandler,19)
{
/* In order to detect unexpected events during development,
   it is recommended to set a breakpoint on the following instruction.
*/
  while (1);

}

/**
  * @brief Timer2 Capture/Compare Interrupt routine.
  * @par Parameters:
  * None
  * @retval 
  * None
  */
INTERRUPT_HANDLER(TIM2_CAP_IRQHandler,20)
{
/* In order to detect unexpected events during development,
   it is recommended to set a breakpoint on the following instruction.
*/
  while (1);

}


/**
  * @brief Timer3 Update/Overflow/Trigger/Break Interrupt routine.
  * @par Parameters:
  * None
  * @retval 
  * None
  */
INTERRUPT_HANDLER(TIM3_UPD_OVF_TRG_BRK_IRQHandler,21)
{
/* In order to detect unexpected events during development,
   it is recommended to set a breakpoint on the following instruction.
*/
  while (1);

}
/**
  * @brief Timer3 Capture/Compare Interrupt routine.
  * @par Parameters:
  * None
  * @retval 
  * None
  */
INTERRUPT_HANDLER(TIM3_CAP_IRQHandler,22)
{
/* In order to detect unexpected events during development,
   it is recommended to set a breakpoint on the following instruction.
*/
  while (1);

}
/**
  * @brief TIM1 Update/Overflow/Trigger/Commutation Interrupt routine.
  * @par Parameters:
  * None
  * @retval 
  * None
  */
INTERRUPT_HANDLER(TIM1_UPD_OVF_TRG_COM_IRQHandler,23)
{
/* In order to detect unexpected events during development,
   it is recommended to set a breakpoint on the following instruction.
*/
  while (1);

}
/**
  * @brief TIM1 Capture/Compare Interrupt routine.
  * @par Parameters:
  * None
  * @retval 
  * None
  */
INTERRUPT_HANDLER(TIM1_CAP_IRQHandler,24)
{
/* In order to detect unexpected events during development,
   it is recommended to set a breakpoint on the following instruction.
*/
  while (1);

}

/**
  * @brief TIM4 Update/Overflow/Trigger Interrupt routine.
  * @par Parameters:
  * None
  * @retval 
  * None
  */
INTERRUPT_HANDLER(TIM4_UPD_OVF_TRG_IRQHandler,25)
{
/* In order to detect unexpected events during development,
   it is recommended to set a breakpoint on the following instruction.
*/
  while (1);

}
/**
  * @brief SPI1 Interrupt routine.
  * @par Parameters:
  * None
  * @retval 
  * None
  */
INTERRUPT_HANDLER(SPI1_IRQHandler,26)
{
/* In order to detect unexpected events during development,
   it is recommended to set a breakpoint on the following instruction.
*/
  while (1);

}

/**
  * @brief USART1 TX Interrupt routine.
  * @par Parameters:
  * None
  * @retval 
  * None
  */
INTERRUPT_HANDLER(USART1_TX_IRQHandler,27)
{
/* In order to detect unexpected events during development,
   it is recommended to set a breakpoint on the following instruction.
*/
  while (1);

}

/**
  * @brief USART1 RX Interrupt routine.
  * @par Parameters:
  * None
  * @retval 
  * None
  */
INTERRUPT_HANDLER(USART1_RX_IRQHandler,28)
{
/* In order to detect unexpected events during development,
   it is recommended to set a breakpoint on the following instruction.
*/
  while (1);

}

/**
  * @brief I2C1 Interrupt routine.
  * @par Parameters:
  * None
  * @retval 
  * None
  */
INTERRUPT_HANDLER(I2C1_IRQHandler,29)
{
/* In order to detect unexpected events during development,
   it is recommended to set a breakpoint on the following instruction.
*/
  while (1);
}

/**
  * @}
  */
/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/

