/**
  ******************************************************************************
  * @file    UART/LPUART_WakeUpFromStop/Src/main.c 
  * @author  MCD Application Team
  * @brief   This sample code shows how to use UART HAL API (LPUART Instance)
  *          to wake up the MCU from STOP mode  
  *          Two boards are used, one which enters STOP mode and the second
  *          one which sends the wake-up stimuli.  
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

/** @addtogroup LPUART_WakeUpFromStop
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* to enable for the board entering STOP mode,
   to disable for the board sending wake-up stimuli */
#define BOARD_IN_STOP_MODE

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* UART handler declaration */
UART_HandleTypeDef UartHandle;
UART_WakeUpTypeDef WakeUpSelection; 
__IO uint32_t UserButtonStatus = 0;  /* set to 1 after User Button interrupt  */
     
/* Buffer used for confirmation messages transmission */
uint8_t aTxBuffer1[] = "RXNE wake-up successful";
uint8_t aTxBuffer2[] = "Start bit detection wake-up successful";
uint8_t aTxBuffer3[] = "7-bit address match wake-up successful";
uint8_t aTxBuffer4[] = "4-bit address match wake-up successful";

uint8_t aTxBuffer[] = "Start bit detection wake-up successful";
uint8_t aWakeUpTrigger1[] = "R";
uint8_t aWakeUpTrigger2[] = "S";
uint8_t aWakeUpTrigger3[] = {0xA9};
uint8_t aWakeUpTrigger4[] = {0x82};


/* Buffer used for reception */
uint8_t aRxBuffer[RXBUFFERSIZE];

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
#if defined(BOARD_IN_STOP_MODE)
static void SystemClock_Config_fromSTOP(void);
#endif
static void Error_Handler(void);
#if !defined(BOARD_IN_STOP_MODE)
static uint16_t Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);
#endif /* !defined(BOARD_IN_STOP_MODE) */
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{

  /* STM32L0xx HAL library initialization:
       - Configure the Flash prefetch
       - Configure the Systick to generate an interrupt each 1 msec
       - Low Level Initialization
     */
  HAL_Init();
  
  /* Configure LED3 */
  BSP_LED_Init(LED3);
  BSP_LED_Init(LED4);

  /* Configure the system clock to 48 MHz */
  SystemClock_Config();
 
  /*##-1- Configure the LPUART peripheral ######################################*/
  /* Put the USART peripheral in the Asynchronous mode (UART Mode) */
  /* LPUART configured as follows:
      - Word Length = 8 Bits
      - Stop Bit = One Stop bit
      - Parity = None
      - BaudRate = 9600 baud
      - Hardware flow control disabled (RTS and CTS signals) */
   
  UartHandle.Instance        = USARTx;
  HAL_UART_DeInit(&UartHandle); 

  UartHandle.Init.BaudRate   = 9600;
  UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
  UartHandle.Init.StopBits   = UART_STOPBITS_1;
  UartHandle.Init.Parity     = UART_PARITY_NONE;
  UartHandle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
  UartHandle.Init.Mode       = UART_MODE_TX_RX;
  UartHandle.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  
  
  if(HAL_UART_Init(&UartHandle) != HAL_OK)
  {
    Error_Handler();
  }
  
#ifdef BOARD_IN_STOP_MODE
  
  BSP_LED_On(LED3);
  /* wait for two seconds before test start */
  HAL_Delay(2000);
  
  /* make sure that no LPUART transfer is on-going */ 
  while(__HAL_UART_GET_FLAG(&UartHandle, USART_ISR_BUSY) == SET);
  /* make sure that UART is ready to receive
  * (test carried out again later in HAL_UARTEx_StopModeWakeUpSourceConfig) */   
  while(__HAL_UART_GET_FLAG(&UartHandle, USART_ISR_REACK) == RESET);

  /* set the wake-up event:
   * specify wake-up on RXNE flag */
  WakeUpSelection.WakeUpEvent = UART_WAKEUP_ON_READDATA_NONEMPTY;
 if (HAL_UARTEx_StopModeWakeUpSourceConfig(&UartHandle, WakeUpSelection)!= HAL_OK)
  {
    Error_Handler(); 
  }
 
  /* Enable the LPUART Wake UP from stop mode Interrupt */
  __HAL_UART_ENABLE_IT(&UartHandle, UART_IT_WUF);
  
  /* about to enter stop mode: switch off LED */
  BSP_LED_Off(LED3);
  /* enable MCU wake-up by LPUART */
  HAL_UARTEx_EnableStopMode(&UartHandle); 
  /* enter stop mode */
  HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);

  /* ... STOP mode ... */  
  
  SystemClock_Config_fromSTOP();
  /* at that point, MCU has been awoken: the LED has been turned back on */
  /* Wake Up based on RXNE flag successful */ 
  HAL_UARTEx_DisableStopMode(&UartHandle);

  /* wait for some delay */
  HAL_Delay(100);
  
  /* Inform other board that wake up is successful */
  if (HAL_UART_Transmit(&UartHandle, (uint8_t*)aTxBuffer1, COUNTOF(aTxBuffer1)-1, 5000)!= HAL_OK)  
  {
    Error_Handler();
  }
  
  /*##-2- Wake Up second step  ###############################################*/
  /* make sure that no UART transfer is on-going */ 
  
  BSP_LED_On(LED3);
  /* wait for two seconds before test start */
  HAL_Delay(2000);
  
  while(__HAL_UART_GET_FLAG(&UartHandle, USART_ISR_BUSY) == SET);
  /* make sure that LPUART is ready to receive 
   * (test carried out again later in HAL_UARTEx_StopModeWakeUpSourceConfig) */    
  while(__HAL_UART_GET_FLAG(&UartHandle, USART_ISR_REACK) == RESET);
  
  /* set the wake-up event:
   * specify wake-up on start-bit detection */
  WakeUpSelection.WakeUpEvent = UART_WAKEUP_ON_STARTBIT;
  if (HAL_UARTEx_StopModeWakeUpSourceConfig(&UartHandle, WakeUpSelection)!= HAL_OK)
  {
    Error_Handler(); 
  }

  /* Enable the LPUART Wake UP from stop mode Interrupt */
  __HAL_UART_ENABLE_IT(&UartHandle, UART_IT_WUF);
  
  /* about to enter stop mode: switch off LED */
  BSP_LED_Off(LED3);
  
  /* enable MCU wake-up by LPUART */
  HAL_UARTEx_EnableStopMode(&UartHandle); 
  /* enter stop mode */
  HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);

  /* ... STOP mode ... */
   
  SystemClock_Config_fromSTOP();  
  /* at that point, MCU has been awoken: the LED has been turned back on */
  /* Wake Up on start bit detection successful */ 
  HAL_UARTEx_DisableStopMode(&UartHandle);
  
  /* wait for some delay */
  HAL_Delay(100);
  
  /* Inform other board that wake up is successful */
  if (HAL_UART_Transmit(&UartHandle, (uint8_t*)aTxBuffer2, COUNTOF(aTxBuffer2)-1, 5000)!= HAL_OK)  
  {
    Error_Handler();
  }
  
  /*##-3- Wake Up third step  ################################################*/
 /* make sure that no LPUART transfer is on-going */ 
  while(__HAL_UART_GET_FLAG(&UartHandle, USART_ISR_BUSY) == SET);
  /* make sure that UART is ready to receive
   * (test carried out again later in HAL_UARTEx_StopModeWakeUpSourceConfig) */       
  while(__HAL_UART_GET_FLAG(&UartHandle, USART_ISR_REACK) == RESET);
     
  /* set the wake-up event:  
   * specify address-to-match type. 
   * The address is 0x29, meaning the character triggering the 
   * address match is 0xA9 */
  WakeUpSelection.WakeUpEvent = UART_WAKEUP_ON_ADDRESS;
  WakeUpSelection.AddressLength = UART_ADDRESS_DETECT_7B; 
  WakeUpSelection.Address = 0x29;  
  if (HAL_UARTEx_StopModeWakeUpSourceConfig(&UartHandle, WakeUpSelection)!= HAL_OK)
  {
    Error_Handler(); 
  }

  /* Enable the LPUART Wake UP from stop mode Interrupt */
  __HAL_UART_ENABLE_IT(&UartHandle, UART_IT_WUF);
  
  /* about to enter stop mode: switch off LED */
  BSP_LED_Off(LED3);
  /* enable MCU wake-up by LPUART */
  HAL_UARTEx_EnableStopMode(&UartHandle); 
  /* enter stop mode */
  HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);

  /* ... STOP mode ... */
   
  SystemClock_Config_fromSTOP();  
  /* at that point, MCU has been awoken: the LED has been turned back on */
  /* Wake Up on 7-bit address detection successful */ 
  HAL_UARTEx_DisableStopMode(&UartHandle);
  /* wait for some delay */
  HAL_Delay(100);
  /* Inform other board that wake up is successful */
  if (HAL_UART_Transmit(&UartHandle, (uint8_t*)aTxBuffer3, COUNTOF(aTxBuffer3)-1, 5000)!= HAL_OK)  
  {
    Error_Handler();
  } 
  
  /*##-4- Wake Up fourth step  ###############################################*/   
 /* make sure that no LPUART transfer is on-going */ 
  while(__HAL_UART_GET_FLAG(&UartHandle, USART_ISR_BUSY) == SET);
  /* make sure that UART is ready to receive
   * (test carried out again later in HAL_UARTEx_StopModeWakeUpSourceConfig) */      
  while(__HAL_UART_GET_FLAG(&UartHandle, USART_ISR_REACK) == RESET);
    
  /* set the wake-up event:  
   * specify address-to-match type. 
   * The address is 0x2, meaning the character triggering the 
   * address match is 0x82 */
  WakeUpSelection.WakeUpEvent = UART_WAKEUP_ON_ADDRESS;
  WakeUpSelection.AddressLength = UART_ADDRESS_DETECT_4B; 
  WakeUpSelection.Address = 0x2;  
  if (HAL_UARTEx_StopModeWakeUpSourceConfig(&UartHandle, WakeUpSelection)!= HAL_OK)
  {
    Error_Handler(); 
  }

  /* Enable the LPUART Wake UP from stop mode Interrupt */
  __HAL_UART_ENABLE_IT(&UartHandle, UART_IT_WUF);
  
  /* about to enter stop mode: switch off LED */
  BSP_LED_Off(LED3);
  /* enable MCU wake-up by LPUART */
  HAL_UARTEx_EnableStopMode(&UartHandle); 
  /* enter stop mode */
  HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);

  /* ... STOP mode ... */
  
  SystemClock_Config_fromSTOP();
  /* at that point, MCU has been awoken: the LED has been turned back on */
  /* Wake Up on 4-bit address detection successful */ 
  /* wait for some delay */
  HAL_Delay(100);
  /* Inform other board that wake up is successful */
  if (HAL_UART_Transmit(&UartHandle, (uint8_t*)aTxBuffer4, COUNTOF(aTxBuffer4)-1, 5000)!= HAL_OK)  
  {
    Error_Handler();
  } 

  
#else

/* initialize the User push-button in Interrupt mode */
  BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_EXTI);
  
  /* Wait for User push-button press before starting the test.
     In the meantime, LED3 is blinking */
  while(UserButtonStatus == 0)
  {
      /* Toggle LED3 */
      BSP_LED_Toggle(LED3); 
      HAL_Delay(100);
  }

  /*##-2- Send the wake-up from stop mode first trigger ######################*/
  /*      (RXNE flag setting)                                                 */
  BSP_LED_On(LED3);
  if(HAL_UART_Transmit(&UartHandle, (uint8_t*)aWakeUpTrigger1, COUNTOF(aWakeUpTrigger1)-1, 5000)!= HAL_OK)
  {
    Error_Handler();
  }
  
  /* Put LPUART peripheral in reception process to wait for other board
     wake up confirmation */  
  if(HAL_UART_Receive(&UartHandle, (uint8_t *)aRxBuffer, COUNTOF(aTxBuffer1)-1, 10000) != HAL_OK)
  {
    Error_Handler();
  } 
  BSP_LED_Off(LED3);
   
  /* Compare the expected and received buffers */
  if(Buffercmp((uint8_t*)aTxBuffer1,(uint8_t*)aRxBuffer,COUNTOF(aTxBuffer1)-1))
  {
    Error_Handler();
  } 

  /* wait for two seconds before test second step */
  HAL_Delay(2000);
  
  /*##-3- Send the wake-up from stop mode second trigger #####################*/
  /*      (start Bit detection)                                               */
  BSP_LED_On(LED3);  
  if(HAL_UART_Transmit(&UartHandle, (uint8_t*)aWakeUpTrigger2, COUNTOF(aWakeUpTrigger2)-1, 5000)!= HAL_OK)
  {
    Error_Handler();
  }

  /* Put LPUART peripheral in reception process to wait for other board
     wake up confirmation */  
  if(HAL_UART_Receive(&UartHandle, (uint8_t *)aRxBuffer, COUNTOF(aTxBuffer2)-1, 10000) != HAL_OK)
  {
    Error_Handler();
  } 
  BSP_LED_Off(LED3);
   
  /* Compare the expected and received buffers */
  if(Buffercmp((uint8_t*)aTxBuffer2,(uint8_t*)aRxBuffer,COUNTOF(aTxBuffer2)-1))
  {
    Error_Handler();
  } 

  /* wait for two seconds before test third step */
  HAL_Delay(2000);
  

  /*##-4- Send the wake-up from stop mode third trigger ######################*/
  /*      (7-bit address match)                                               */ 
  BSP_LED_On(LED3);  
  if(HAL_UART_Transmit(&UartHandle, (uint8_t*)aWakeUpTrigger3, 1, 5000)!= HAL_OK)
  {
    Error_Handler();
  }
 
  /* Put LPUART peripheral in reception process to wait for other board
     wake up confirmation */  
  if(HAL_UART_Receive(&UartHandle, (uint8_t *)aRxBuffer, COUNTOF(aTxBuffer3)-1, 10000) != HAL_OK)
  {
    Error_Handler();
  } 
  BSP_LED_Off(LED3);
   
  /* Compare the expected and received buffers */
  if(Buffercmp((uint8_t*)aTxBuffer3,(uint8_t*)aRxBuffer,COUNTOF(aTxBuffer3)-1))
  {
    Error_Handler();
  } 

  /* wait for two seconds before test fourth and last step */
  HAL_Delay(2000);


  /*##-5- Send the wake-up from stop mode fourth trigger #####################*/
  /*      (4-bit address match)                                               */  
  BSP_LED_On(LED3); 
  if(HAL_UART_Transmit(&UartHandle, (uint8_t*)aWakeUpTrigger4, 1, 5000)!= HAL_OK)
  {
    Error_Handler();
  }
 
  
  /* Put LPUART peripheral in reception process to wait for other board
     wake up confirmation */  
  if(HAL_UART_Receive(&UartHandle, (uint8_t *)aRxBuffer, COUNTOF(aTxBuffer4)-1, 10000) != HAL_OK)
  {
    Error_Handler();
  } 
  BSP_LED_Off(LED3);
   
  /* Compare the expected and received buffers */
  if(Buffercmp((uint8_t*)aTxBuffer4,(uint8_t*)aRxBuffer,COUNTOF(aTxBuffer4)-1))
  {
    Error_Handler();
  } 

  HAL_Delay(2000);

#endif /* BOARD_IN_STOP_MODE */
 
  /* Turn on LED3 & LED4 if test passes then enter infinite loop */
  BSP_LED_On(LED3); 
  BSP_LED_On(LED4); 
  while (1)
  {
  }
}


/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSI)
  *            SYSCLK(Hz)                     = 32000000
  *            HCLK(Hz)                       = 32000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            APB2 Prescaler                 = 1
  *            HSI Frequency(Hz)              = 16000000
  *            PLL_MUL                        = 4
  *            PLL_DIV                        = 2
  *            Flash Latency(WS)              = 1
  *            Main regulator output voltage  = Scale1 mode
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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /* Enable HSI Oscillator and activate PLL with HSI as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSEState = RCC_HSE_OFF;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL4;
  RCC_OscInitStruct.PLL.PLLDIV = RCC_PLL_DIV2;
  RCC_OscInitStruct.HSICalibrationValue = 0x10;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);  
  
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;  
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1);
}

#if defined(BOARD_IN_STOP_MODE)
static void SystemClock_Config_fromSTOP(void)
{
    SystemClock_Config();
}
#endif

/**
  * @brief  UART error callbacks
  * @param  UartHandle: UART handle
  * @note   This example shows a simple way to report transfer error, and you can
  *         add your own implementation.
  * @retval None
  */
void HAL_UART_ErrorCallback(UART_HandleTypeDef *UartHandle)
{
  Error_Handler();
}

/**
  * @brief EXTI line detection callbacks
  * @param GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if(GPIO_Pin == KEY_BUTTON_PIN)
  {  
    UserButtonStatus = 1;
  }
}

/**
  * @brief UART wakeup from Stop mode callback
  * @param huart: uart handle
  * @retval None
  */
void HAL_UARTEx_WakeupCallback(UART_HandleTypeDef *huart)
{
  /* Turn back on LED3 */
    BSP_LED_On(LED3);
}
#if !defined(BOARD_IN_STOP_MODE)
/**
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval 0  : pBuffer1 identical to pBuffer2
  *         >0 : pBuffer1 differs from pBuffer2
  */
static uint16_t Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
{
  while (BufferLength--)
  {
    if ((*pBuffer1) != *pBuffer2)
    {
      return BufferLength;
    }
    pBuffer1++;
    pBuffer2++;
  }

  return 0;
}
#endif /* !defined(BOARD_IN_STOP_MODE) */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  while(1)
  {
    /* In case of error, LED3 transmits a sequence of three dots, three dashes, three dots */
    BSP_LED_On(LED3); 
    HAL_Delay(300);
    BSP_LED_Off(LED3);
    HAL_Delay(300); 
    BSP_LED_On(LED3); 
    HAL_Delay(300);
    BSP_LED_Off(LED3);
    HAL_Delay(300);  
    BSP_LED_On(LED3); 
    HAL_Delay(300);
    BSP_LED_Off(LED3);
    HAL_Delay(300);   
    BSP_LED_On(LED3); 
    HAL_Delay(700);
    BSP_LED_Off(LED3);
    HAL_Delay(700); 
    BSP_LED_On(LED3); 
    HAL_Delay(700);
    BSP_LED_Off(LED3);
    HAL_Delay(700);  
    BSP_LED_On(LED3); 
    HAL_Delay(700);
    BSP_LED_Off(LED3);
    HAL_Delay(700); 
    BSP_LED_On(LED3); 
    HAL_Delay(300);
    BSP_LED_Off(LED3);
    HAL_Delay(300); 
    BSP_LED_On(LED3); 
    HAL_Delay(300);
    BSP_LED_Off(LED3);
    HAL_Delay(300);  
    BSP_LED_On(LED3); 
    HAL_Delay(300);
    BSP_LED_Off(LED3);
    HAL_Delay(800); 
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
