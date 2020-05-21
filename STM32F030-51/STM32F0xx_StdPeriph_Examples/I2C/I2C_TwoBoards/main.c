/**
  ******************************************************************************
  * @file    I2C/I2C_TwoBoards/main.c 
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

/** @addtogroup I2C_TwoBoards
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
   
/* CPAL local transfer structures */
CPAL_TransferTypeDef  sRxStructure, sTxStructure;

/* Buffers tables declarations */
const uint8_t tStateSignal[]  = "STM32 CPAL 2xBoards Example: Signal State  ";
const uint8_t tSignal1[]      = "STM32 CPAL 2xBoards Example: Signal Signal1";
const uint8_t tSignal2[]      = "STM32 CPAL 2xBoards Example: Signal Signal2";

uint8_t tRxBuffer[MAX_BUFF_SIZE];
uint32_t BufferSize = BUFFER_SIZE;
uint8_t Color = 0;
__IO uint32_t ActionState = ACTION_NONE;
__IO uint32_t RecieverMode = 0;
__IO uint32_t TransmitMode = 0;
uint32_t PeriodicValue = 15000, Divider = 1;

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
static void TIM6_Config(void);
static void TIM17_Config(uint32_t Period);
uint8_t Buffer_Check(uint8_t* pBuffer, uint8_t* pBuffer1, uint8_t* pBuffer2,  uint8_t* pBuffer3, uint16_t BufferLength);
TestStatus Buffer_Compare(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);
void Switch_Color(void);
void Switch_ErrorColor(void);


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
  
  /* Initialize LEDs and LCD available on STM320518-EVAL board ****************/
  STM_EVAL_LEDInit(LED1);
  STM_EVAL_LEDInit(LED2);
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED4);
    
  /* Initialize TIM6 */
  TIM6_Config();
  
  /* Initialize the LCD */
#ifdef USE_STM320518_EVAL
  STM320518_LCD_Init();
#else 
  STM32072B_LCD_Init();
#endif /* USE_STM320518_EVAL */

  /* Display message on  LCD **************************************************/
  /* Clear the LCD */ 
  LCD_Clear(White);  
  /* Set the LCD Back Color */
  LCD_SetBackColor(Blue);
  /* Set the LCD Text Color */
  LCD_SetTextColor(Yellow);
  LCD_DisplayStringLine(Line0, MESSAGE1);
  LCD_DisplayStringLine(Line1, MESSAGE2);
  LCD_DisplayStringLine(Line3, MESSAGE3);
  
  /* Set the LCD Back Color */
  LCD_SetBackColor(White);
  /* Set the LCD Text Color */
  LCD_SetTextColor(Blue);
  
  /* Configure the Push buttons in interrupt mode *****************************/
  STM_EVAL_PBInit(BUTTON_SEL, Mode_EXTI);
  STM_EVAL_PBInit(BUTTON_TAMPER, Mode_EXTI);
  
  /* Start CPAL communication configuration ***********************************/
  /* Initialize local Reception structures */
  sRxStructure.wNumData = BufferSize;       /* Maximum Number of data to be received */
  sRxStructure.pbBuffer = tRxBuffer;        /* Common Rx buffer for all received data */
  sRxStructure.wAddr1 = 0;                  /* Not needed */
  sRxStructure.wAddr2 = 0;                  /* Not needed */
  
  /* Initialize local Transmission structures */
  sTxStructure.wNumData = BufferSize;       /* Maximum Number of data to be received */
  sTxStructure.pbBuffer = (uint8_t*)tStateSignal;     /* Common Rx buffer for all received data */
  sTxStructure.wAddr1 = OWN_ADDRESS;        /* The own board address */
  sTxStructure.wAddr2 = 0;                  /* Not needed */
  
  /* Set SYSCLK as I2C clock source */
  RCC_I2CCLKConfig(RCC_I2C1CLK_SYSCLK);
  
  /* Configure the device structure */
  CPAL_I2C_StructInit(&I2C_DevStructure);      /* Set all fields to default values */
  I2C_DevStructure.CPAL_Mode = CPAL_MODE_SLAVE;
#ifdef CPAL_I2C_DMA_PROGMODEL
  I2C_DevStructure.wCPAL_Options =  CPAL_OPT_NO_MEM_ADDR | CPAL_OPT_DMATX_TCIT | CPAL_OPT_DMARX_TCIT;
  I2C_DevStructure.CPAL_ProgModel = CPAL_PROGMODEL_DMA;
#elif defined (CPAL_I2C_IT_PROGMODEL)
  I2C_DevStructure.wCPAL_Options =  CPAL_OPT_NO_MEM_ADDR;
  I2C_DevStructure.CPAL_ProgModel = CPAL_PROGMODEL_INTERRUPT;
#else
 #error "Please select one of the programming model (in stm32f0xx_i2c_cpal_conf.h)"
#endif
  I2C_DevStructure.pCPAL_I2C_Struct->I2C_Timing = I2C_TIMING;
  I2C_DevStructure.pCPAL_I2C_Struct->I2C_OwnAddress1 = OWN_ADDRESS;
  I2C_DevStructure.pCPAL_TransferRx = &sRxStructure;
  I2C_DevStructure.pCPAL_TransferTx = &sTxStructure;
  
  /* Initialize CPAL device with the selected parameters */
  CPAL_I2C_Init(&I2C_DevStructure);    
  
  /* Infinite loop */
  while (1)
  {
    /* Write operations ------------------------------------------------------*/
    /* Check if any action has been triggered by push buttons */
    if ((ActionState != ACTION_PENDING) && (ActionState != ACTION_NONE))
    {
      /* Check if the current CPAL device state allows write operation */
      if ((I2C_DevStructure.CPAL_State == CPAL_STATE_READY) || \
        (I2C_DevStructure.CPAL_State == CPAL_STATE_BUSY_RX) ||\
          (I2C_DevStructure.CPAL_State == CPAL_STATE_DISABLED))
      {        
        /* Initialize local Transmission structures */
        sTxStructure.wNumData = BufferSize;   /* Maximum Number of data to be received */
        sTxStructure.wAddr1 = OWN_ADDRESS;    /* The own board address */
        sTxStructure.wAddr2 = 0;              /* Not needed */        
        
        switch (ActionState)
        {
          
        case BUTTON_SEL: 
          sTxStructure.pbBuffer = (uint8_t*)tSignal1; 
          Divider = 1;
          break;
          
        case BUTTON_TAMPER:
          sTxStructure.pbBuffer = (uint8_t*)tSignal2; 
          Divider = 2;
          break;
          
        case ACTION_PERIODIC:
          sTxStructure.pbBuffer = (uint8_t*)tStateSignal; 
          break;    
          
        default:
          sTxStructure.pbBuffer = (uint8_t*)tSignal1; 
          break;            
        } 
        
        /* Configure the device mode to master */
        I2C_DevStructure.CPAL_Mode = CPAL_MODE_MASTER;
        /* Force the CPAL state to ready (in case a read operation has been initiated) */
        I2C_DevStructure.CPAL_State = CPAL_STATE_READY;
                
        /* Prevent other actions to be performed while the current is not finished */
        ActionState = ACTION_PENDING;
        TransmitMode = STATE_ON;
        
        /* Configure a Timer to generate periodic interrupt: used to send state signal */
        TIM17_Config(PeriodicValue/Divider);   
        
        /* Start writing data in master mode */
        if (CPAL_I2C_Write(&I2C_DevStructure) == CPAL_PASS)
        {
         }
      }      
    }
    
    
    /* Read Operations -------------------------------------------------------*/
    if (((I2C_DevStructure.CPAL_State == CPAL_STATE_READY) || \
      (I2C_DevStructure.CPAL_State == CPAL_STATE_DISABLED)) && \
        (TransmitMode == STATE_OFF))
    {                  
      /* Initialize local Reception structures */
      sRxStructure.wNumData = BufferSize;       /* Maximum Number of data to be received */
      sRxStructure.pbBuffer = tRxBuffer;        /* Common Rx buffer for all received data */
      
      /* Reconfigure device for slave receiver mode */
      I2C_DevStructure.CPAL_Mode = CPAL_MODE_SLAVE;
      I2C_DevStructure.CPAL_State = CPAL_STATE_READY;
      
      /* Start waiting for data to be received in slave mode */
      if (CPAL_I2C_Read(&I2C_DevStructure) == CPAL_PASS)
      {
        LCD_DisplayStringLine(Line9, MEASSAGE_EMPTY); 
      }
    }   
  }
}
/**
  * @brief  Configures TIM6 and associated resources to generate an update interrupt each 50 ms.
  * @param  None
  * @retval None
  */
static void TIM6_Config(void)
{	  
  /* TIM6 is used to toggle Led1 and Led4 each 50ms */  
  
  NVIC_InitTypeDef NVIC_InitStructure;
  TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;
  
  /* TIMER clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6 , ENABLE);
  
  /* Enable the TIMER global Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM6_DAC_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);  
  
  TIM_TimeBaseStructure.TIM_Period  = 50000;
  TIM_TimeBaseStructure.TIM_Prescaler = (SystemCoreClock/1000000)-1; 
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode =  TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);
  
  /* TIM IT enable */
  TIM_ITConfig(TIM6, TIM_IT_Update , ENABLE);
  
  /* TIM17 enable counter */
  TIM_Cmd(TIM6, ENABLE);
}

/**
  * @brief  Configures TIM17 and associated resources to generate an update interrupt.
  *         The period of Timer is depending on Period value.
  * @param  Period : affected to Timer Period
  * @retval None
  */
static void TIM17_Config(uint32_t Period)
{	  
  /* TIM17 is used to generate periodic interrupts. At each interrupt 
  if Transmitter mode is selected, a status message is sent to other Board */
  
  NVIC_InitTypeDef NVIC_InitStructure;
  TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;
  
  /* TIMER clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM17 , ENABLE);
  
  NVIC_InitStructure.NVIC_IRQChannel = TIM17_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0x01;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);  
  
  TIM_TimeBaseStructure.TIM_Period  = Period;
  TIM_TimeBaseStructure.TIM_Prescaler = (SystemCoreClock/10000)-1; 
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode =  TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM17, &TIM_TimeBaseStructure);
  
  /* TIM IT enable */
  TIM_ITConfig(TIM17, TIM_IT_Update , ENABLE);
  
  /* TIM17 enable counter */
  TIM_Cmd(TIM17, ENABLE);
}
/**
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval PASSED: pBuffer1 identical to pBuffer2
  *         FAILED: pBuffer1 differs from pBuffer2
  */
TestStatus Buffer_Compare(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
{
  while(BufferLength--)
  {
    if(*pBuffer1 != *pBuffer2)
    {
      return FAILED;
    }
    
    pBuffer1++;
    pBuffer2++;
  }

  return PASSED;  
}

/**
  * @brief  Compares a buffer to a list of buffers.
  * @param  pBuffer, pBuffer1: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval PASSED: pBuffer identical to pBuffer1
  *   FAILED: pBuffer differs from pBuffer1
  */

uint8_t Buffer_Check(uint8_t* pBuffer, uint8_t* pBuffer1, uint8_t* pBuffer2,  uint8_t* pBuffer3, uint16_t BufferLength)
{ 
  if (Buffer_Compare(pBuffer, pBuffer1, BufferLength) == PASSED)
  {
    return 0;
  }
  else if (Buffer_Compare(pBuffer, pBuffer2, BufferLength) == PASSED)
  {
    return 1;
  }
  else if (Buffer_Compare(pBuffer, pBuffer3, BufferLength) == PASSED)
  {
    return 2;
  }  
  else
  {
    return 0xFF; /* Non identified buffer */
  }
}

/**
  * @brief  Swap the color of the line on LCD.
  * @param  None.
  * @retval None.
  */
void Switch_Color(void)
{
  /* Set the message color */
  if(Color == 0)
  {
    LCD_SetTextColor(Black);
    Color=1;
  }
  else
  {
    LCD_SetTextColor(Blue2);
    Color=0; 
  }   
}

/**
  * @brief  Swap the color of the line on LCD.
  * @param  None.
  * @retval None.
  */
void Switch_ErrorColor(void)
{
  /* Set the message color */
  if(Color == 0)
  {
    LCD_SetTextColor(Magenta);
    Color=1;
  }
  else
  {
    LCD_SetTextColor(Red);
    Color=0; 
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
