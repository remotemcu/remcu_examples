/**
  ******************************************************************************
  * @file    FLASH/Dual_Boot/main.c
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"


/** @addtogroup STM32F10x_StdPeriph_Examples
  * @{
  */

/** @addtogroup Dual_Boot
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

/* Private define ------------------------------------------------------------*/
/* Uncomment one of the lines below to select which bank to boot from  */
#if !defined(BOOT_FROM_BANK1) && !defined(BOOT_FROM_BANK2)
//#define BOOT_FROM_BANK1           /* The program will be loaded on Flash Bank1 */
//#define BOOT_FROM_BANK2           /* The program will be loaded on Flash Bank2 */
#endif

#if defined(BOOT_FROM_BANK1)
 #define MESSAGE4   " Running from Bank 1"
#elif defined(BOOT_FROM_BANK2)
 #define MESSAGE4   " Running from Bank 2"
#else
 #error "Select Boot from Bank1 or Bank2 using defines:BOOT_FROM_BANK1 or BOOT_FROM_BANK2!"
#endif

#define MESSAGE1   "  STM32 XL Density  "
#define MESSAGE2   " Device running on  "
#define MESSAGE3   "   STM3210E-EVAL    "

#define MESSAGE5   " Joystick-DOWN: reset BFB2"
#define MESSAGE6   "  bit to Boot from Bank2  "
#define MESSAGE7   " Joystick-UP: set BFB2    "
#define MESSAGE8   "  bit to Boot from Bank1  "

#define MESSAGE9   " Joystick-SEL: program to "
#define MESSAGE10  " 0x0 the base @ of Bank1/2"

#define MESSAGE11   "  Operation Failed !"
#define MESSAGE12   "Bank 1/2 base @ -> 0"

#define BANK1_START_ADDRESS       0x08000000
#define BANK2_START_ADDRESS       0x08080000

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
USART_InitTypeDef USART_InitStructure;

static __IO uint32_t TimingDelay;
RCC_ClocksTypeDef RCC_Clocks;

/* Private function prototypes -----------------------------------------------*/
void Delay(__IO uint32_t nTime);

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
       file (startup_stm32f10x_xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f10x.c file
     */

  /* Set the vector table address */
#if defined(BOOT_FROM_BANK1)
  /* Set the vector table to the Bank1 start address */
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, BANK1_START_ADDRESS);
#elif defined(BOOT_FROM_BANK2)
  /* Set the vector table to the Bank1 start address */
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, BANK2_START_ADDRESS);
#endif /* BOOT_FROM_BANK1 */

  /* Initialize LEDs, Buttons and LCD on STM3210E-EVAL board *****************/
  STM_EVAL_LEDInit(LED1);
  STM_EVAL_LEDInit(LED2);
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED4);

  /* SysTick end of count event each 10ms */
  RCC_GetClocksFreq(&RCC_Clocks);
  SysTick_Config(RCC_Clocks.HCLK_Frequency / 100);

  /* Configure the Joystick buttons */
  STM_EVAL_PBInit(BUTTON_UP, BUTTON_MODE_GPIO);
  STM_EVAL_PBInit(BUTTON_SEL, BUTTON_MODE_GPIO);
  STM_EVAL_PBInit(BUTTON_DOWN, BUTTON_MODE_GPIO);
  /* Initialize the LCD */
  STM3210E_LCD_Init();

  /* Display message on STM3210E-EVAL LCD *************************************/
  /* Clear the LCD */
  LCD_Clear(LCD_COLOR_WHITE);

  /* Set the LCD Back Color */
#if defined(BOOT_FROM_BANK1)
  LCD_SetBackColor(LCD_COLOR_BLUE);
#elif defined(BOOT_FROM_BANK2)
  LCD_SetBackColor(LCD_COLOR_RED);
#endif /* BOOT_FROM_BANK1 */

  /* Set the LCD Text Color */
  LCD_SetTextColor(LCD_COLOR_WHITE);
  LCD_DisplayStringLine(LCD_LINE_0, MESSAGE1);
  LCD_DisplayStringLine(LCD_LINE_1, MESSAGE2);
  LCD_DisplayStringLine(LCD_LINE_2, MESSAGE3);
  LCD_DisplayStringLine(LCD_LINE_4, MESSAGE4);

  LCD_SetFont(&Font12x12);
  LCD_DisplayStringLine(LCD_LINE_12, MESSAGE5);
  LCD_DisplayStringLine(LCD_LINE_13, MESSAGE6);
  LCD_DisplayStringLine(LCD_LINE_15, MESSAGE7);
  LCD_DisplayStringLine(LCD_LINE_16, MESSAGE8);
  LCD_DisplayStringLine(LCD_LINE_18, MESSAGE9);
  LCD_DisplayStringLine(LCD_LINE_19, MESSAGE10);
  LCD_SetFont(&Font16x24);

  /* Turn on leds available on STM3210E-EVAL **********************************/
  STM_EVAL_LEDOn(LED1);
  STM_EVAL_LEDOn(LED2);
  STM_EVAL_LEDOn(LED3);
  STM_EVAL_LEDOn(LED4);

  /* Infinite loop */
  while (1)
  {
    /*--- If Joystick DOWN button is pushed, reset BFB2 bit to enable boot from Bank2
          (active after next reset, w/ Boot pins set in Boot from Flash memory position ---*/
    if (STM_EVAL_PBGetState(BUTTON_DOWN) == 0)
    {
      /* Reset BFB2 bit to enable boot from Flash Bank2 */
      FLASH_Unlock();
      FLASH_EraseOptionBytes();

      if (FLASH_BootConfig(FLASH_BOOT_Bank2) == FLASH_COMPLETE)
      {
        /* Generate System Reset to load the new option byte values */
        NVIC_SystemReset();
      }
      else
      {
        /* Display information */
        LCD_DisplayStringLine(LCD_LINE_6, MESSAGE11);
      }
    }

    /*--- If Joystick UP button is pushed, set BFB2 bit to enable boot from Bank1
          (active after next reset, w/ Boot pins set in Boot from Flash memory position ---*/
    if (STM_EVAL_PBGetState(BUTTON_UP) == 0)
    {
      /* Set BFB2 bit to enable boot from Flash Bank2 */
      FLASH_Unlock();
      FLASH_EraseOptionBytes();
      if (FLASH_BootConfig(FLASH_BOOT_Bank1) == FLASH_COMPLETE)
      {
        /* Generate System Reset to load the new option byte values */
        NVIC_SystemReset();
      }
      else
      {
        /* Display information */
        LCD_DisplayStringLine(LCD_LINE_6, MESSAGE11);
      }
    }

    /*--- If Joystick UP button is pushed, program the content of address 0x08080000
          (base address of Bank2) and 0x08000000(base address of Bank1) to 0x00 --*/
    if (STM_EVAL_PBGetState(BUTTON_SEL) == 0)
    {
      FLASH_Unlock();
      /* Erase stack pointer value at Bank 2 start address */
      FLASH_ProgramWord(BANK2_START_ADDRESS, 0x00);
      /* Erase stack pointer value at Bank 1 start address */
      FLASH_ProgramWord(BANK1_START_ADDRESS, 0x00);
      FLASH_Lock();

      LCD_ClearLine(LCD_LINE_7);
      LCD_ClearLine(LCD_LINE_8);
      LCD_ClearLine(LCD_LINE_9);

      /* Check if erase operation is OK */
      if ((uint32_t)(*(uint32_t *)BANK2_START_ADDRESS) ==  0x00)
      {
        if ((uint32_t)(*(uint32_t *)BANK1_START_ADDRESS) !=  0x00)
        {
          /* Display information */
          LCD_DisplayStringLine(LCD_LINE_6, MESSAGE11);
        }
        else
        {
          /* Display information */
          LCD_DisplayStringLine(LCD_LINE_6, MESSAGE12);
        }
      }
      else
      {
        /* Display information */
        LCD_DisplayStringLine(LCD_LINE_6, MESSAGE11);
      }
    }

    /* Toggle LD3 */
    STM_EVAL_LEDToggle(LED3);

    /* Insert 50 ms delay */
    Delay(5);

    /* Toggle LD2 */
    STM_EVAL_LEDToggle(LED2);

    /* Insert 100 ms delay */
    Delay(10);
  }
}

/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in 10 ms.
  * @retval None
  */
void Delay(__IO uint32_t nTime)
{
  TimingDelay = nTime;

  while (TimingDelay != 0);
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  {
    TimingDelay--;
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

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
