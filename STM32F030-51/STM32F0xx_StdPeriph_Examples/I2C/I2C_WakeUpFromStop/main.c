/**
  ******************************************************************************
  * @file    I2C/I2C_WakeUpFromStop/main.c 
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

/** @addtogroup I2C_WakeUpFromStop
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
const uint8_t BufferTX[]= "STM32 CPAL FIRMWARE DRIVER FOR I2Cx DEVICE: WAKEUP FROM STOP MODE EXAMPLE";
uint8_t BufferRX[BUFFSIZE];
CPAL_TransferTypeDef  sRxStructure, sTxStructure;
uint32_t BufferSize = BUFFSIZE;
#ifdef I2C_SLAVE  
 GPIO_InitTypeDef GPIO_InitStructure;
#endif /* I2C_SLAVE */
__IO TestStatus TransferStatus = PASSED;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static void RCC_Config(void);
static __IO uint32_t TimingDelay;
#ifdef I2C_SLAVE  
 static TestStatus Compare_bBuffer(uint8_t* pBuffer, uint8_t* pBuffer1, uint32_t BufferLength);
 static void Reset_bBuffer(uint8_t *pBuffer, uint32_t BufferLenght);
 static void Restore_Configuration(void);
#endif /* I2C_SLAVE */


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
    
  /* Configure Clocks */
  RCC_Config();
  
  /* Initialize LEDsand LCD available on EVAL board ***************************/
  STM_EVAL_LEDInit(LED1);
  STM_EVAL_LEDInit(LED2);
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED4);
  
  /* Initialize the LCD */
#ifdef USE_STM320518_EVAL
  STM320518_LCD_Init();
#else 
  STM32072B_LCD_Init();
#endif /* USE_STM320518_EVAL */

  /* Display message on  LCD ***********************************************/
  /* Clear the LCD */ 
  LCD_Clear(White);  
  /* Set the LCD Back Color */
  LCD_SetBackColor(Blue);
  /* Set the LCD Text Color */
  LCD_SetTextColor(Yellow);
  LCD_DisplayStringLine(Line0, MESSAGE1);
  LCD_DisplayStringLine(Line1, MESSAGE2);  
  /* Set the LCD Back Color */
  LCD_SetBackColor(White);
  /* Set the LCD Text Color */
  LCD_SetTextColor(Blue);
  
  /* Configure the Push buttons in Polling mode */
  STM_EVAL_PBInit(BUTTON_SEL, Mode_GPIO);
  
  /* if STM32 device is set as Master */
#ifdef I2C_MASTER     

  /* Configure and enable the systick timer to generate an interrupt each 1 ms */
  SysTick_Config((SystemCoreClock / 1000));
   
  /* Deinitialize I2Cx Device */ 
  CPAL_I2C_DeInit(&MASTERSTRUCTURE); 
  
  /* Initialize CPAL I2C structure parameters values */
  CPAL_I2C_StructInit(&MASTERSTRUCTURE);
  
 #ifdef CPAL_I2C_DMA_PROGMODEL
   MASTERSTRUCTURE.wCPAL_Options =  CPAL_OPT_NO_MEM_ADDR | CPAL_OPT_DMATX_TCIT;
   MASTERSTRUCTURE.CPAL_ProgModel = CPAL_PROGMODEL_DMA;
 #elif defined (CPAL_I2C_IT_PROGMODEL)
   MASTERSTRUCTURE.wCPAL_Options =  CPAL_OPT_NO_MEM_ADDR;
   MASTERSTRUCTURE.CPAL_ProgModel = CPAL_PROGMODEL_INTERRUPT;
 #else
  #error "Please select one of the programming model (in main.h)"
 #endif
  
  /* Set I2C Speed */
  MASTERSTRUCTURE.pCPAL_I2C_Struct->I2C_Timing = MASTER_I2C_TIMING;
  
  /* Select Master Mode */
  MASTERSTRUCTURE.CPAL_Mode = CPAL_MODE_MASTER; 
  
  /* Initialize I2Cx Device*/
  CPAL_I2C_Init(&MASTERSTRUCTURE); 
  
  /* Infinite loop */
  while(1)
  {   
    /* Initialize Transfer parameters */
    MASTERSTRUCTURE.pCPAL_TransferTx = &sTxStructure;    
    sTxStructure.wNumData = BufferSize;
    sTxStructure.pbBuffer = (uint8_t*)BufferTX;
    sTxStructure.wAddr1 = OWNADDRESS;
    
    /* Update LCD Display */
    LCD_SetBackColor(White);
    LCD_SetTextColor(Blue);    
    LCD_DisplayStringLine(Line8, MEASSAGE_EMPTY);
    LCD_DisplayStringLine(Line5, MESSAGE4);
    LCD_DisplayStringLine(Line6, MESSAGE5);
    
    /* wait until Key button is pushed */
    while(! STM_EVAL_PBGetState(BUTTON_SEL));
    
    /* Update LCD Display */
    LCD_DisplayStringLine(Line5, MEASSAGE_EMPTY);
    LCD_DisplayStringLine(Line6, MEASSAGE_EMPTY);
    
    /* Write operation */
    CPAL_I2C_Write(&MASTERSTRUCTURE);
    
    /* Wait until communication finishes */
    while ((MASTERSTRUCTURE.CPAL_State != CPAL_STATE_READY) && (MASTERSTRUCTURE.CPAL_State != CPAL_STATE_ERROR));
    
    if (TransferStatus == PASSED)
    {
      /* Update LCD Display */
      LCD_SetBackColor(Red);
      LCD_SetTextColor(White);    
      LCD_DisplayStringLine(Line8, MESSAGE6);      
    }
    else
    {
      TransferStatus = PASSED;
    }
    
    Delay(1000);
  }
#endif /* I2C_MASTER */
  
  /* if STM32 device is set as Slave */  
#ifdef I2C_SLAVE    
  
  /* GPIOA Periph clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  
  /* Output System Clock on MCO pin (PA.08) */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
#ifdef USE_STM320518_EVAL
  RCC_MCOConfig(RCC_MCOSource_SYSCLK);
#else 
  RCC_MCOConfig(RCC_MCOSource_SYSCLK, RCC_MCOPrescaler_1);
#endif /* USE_STM320518_EVAL */
  
  /* Deinitialize I2Cx Device */ 
  CPAL_I2C_DeInit(&SLAVESTRUCTURE); 
  
  /* Initialize CPAL I2C structure parameters values */
  CPAL_I2C_StructInit(&SLAVESTRUCTURE);
  
#ifdef CPAL_I2C_DMA_PROGMODEL
  SLAVESTRUCTURE.wCPAL_Options = CPAL_OPT_I2C_NACK_ADD | CPAL_OPT_I2C_WAKEUP_STOP | CPAL_OPT_DMARX_TCIT;
  SLAVESTRUCTURE.CPAL_ProgModel = CPAL_PROGMODEL_DMA;
#elif defined (CPAL_I2C_IT_PROGMODEL)
  SLAVESTRUCTURE.wCPAL_Options =  CPAL_OPT_I2C_NACK_ADD | CPAL_OPT_I2C_WAKEUP_STOP;
  SLAVESTRUCTURE.CPAL_ProgModel = CPAL_PROGMODEL_INTERRUPT;
#else
 #error "Please select one of the programming model (in main.h)"
#endif
  
  /* Configure Own address 1 */
  SLAVESTRUCTURE.pCPAL_I2C_Struct->I2C_OwnAddress1 = OWNADDRESS;
  
  /* Set I2C Speed */
  SLAVESTRUCTURE.pCPAL_I2C_Struct->I2C_Timing = SLAVE_I2C_TIMING;
  
  /* Select Slave Mode */ 
  SLAVESTRUCTURE.CPAL_Mode = CPAL_MODE_SLAVE; 
  
  /* Initialize I2Cx Device*/
  CPAL_I2C_Init(&SLAVESTRUCTURE);    
  
  /* Infinite loop */
  while(1)
  {     
    /* Reset BufferRX value */
    Reset_bBuffer(BufferRX, (uint16_t)BufferSize);
    
    /* Initialize Transfer parameters */
    SLAVESTRUCTURE.pCPAL_TransferRx = &sRxStructure;    
    sRxStructure.wNumData = BufferSize;         
    sRxStructure.pbBuffer = (uint8_t*)BufferRX;
    
    /* Update LCD Display */
    LCD_SetBackColor(White);
    LCD_SetTextColor(Blue);
    LCD_DisplayStringLine(Line8, MEASSAGE_EMPTY);
    LCD_DisplayStringLine(Line9, MEASSAGE_EMPTY);
    LCD_DisplayStringLine(Line5, MESSAGE7);
    
    Delay(1000);
    
    /* Update LCD Display */
    LCD_DisplayStringLine(Line5, MEASSAGE_EMPTY);
    LCD_DisplayStringLine(Line6, MESSAGE8);
    
    /* Read operation */
    CPAL_I2C_Read(&SLAVESTRUCTURE);  
    
    /* Enter Stop Mode and wait for interrupt to wake up */
    PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);
    
    /* Wait until communication finishes */
    while ((SLAVESTRUCTURE.CPAL_State != CPAL_STATE_READY) && (SLAVESTRUCTURE.CPAL_State != CPAL_STATE_ERROR));
    
    /* Configure SystemClock*/
    Restore_Configuration();
    
    /* Configure and enable the systick timer to generate an interrupt each 1 ms */
    SysTick_Config((SystemCoreClock / 1000));
  
    /* Update LCD Display */
    LCD_DisplayStringLine(Line6, MEASSAGE_EMPTY);    
    LCD_SetBackColor(Red);
    LCD_SetTextColor(White);     
    LCD_DisplayStringLine(Line8, MESSAGE9);
    
    /* If are received correctly */
    if (Compare_bBuffer((uint8_t*)BufferTX, BufferRX, BufferSize) == PASSED )
    {          
      /* Update LCD Display */
      LCD_DisplayStringLine(Line9, MESSAGE6);      
    }
    else
    {          
      /* Update LCD Display */
      LCD_DisplayStringLine(Line9, MESSAGE10);
    }
    
    Delay(1500);
  }  
#endif /* I2C_SLAVE */
}


/**
  * @brief  Configure the I2C Clock source and Power clock
  * @param  None
  * @retval None
  */
static void RCC_Config(void)
{
  /* Configure the I2C1 Clock Source */
  RCC_I2CCLKConfig(RCC_I2C1CLK_HSI);
  
  /* Enable PWR APB clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE); 
}

#ifdef I2C_SLAVE  
/**
  * @brief  Restore peripheral config before entering STOP mode.
  * @param  None
  * @retval None
  */
static void Restore_Configuration(void)
{
  __IO uint32_t StartUpCounter = 0, HSEStatus = 0;
  
  /* SYSCLK, HCLK, PCLK configuration ----------------------------------------*/    
  /* Enable HSE */    
  RCC_HSEConfig(RCC_HSE_ON);
 
  /* Wait till HSE is ready and if Time out is reached exit */
  HSEStatus = RCC_WaitForHSEStartUp();

  if (HSEStatus == (uint32_t)0x01)
  {
    /* Enable Prefetch Buffer */
    FLASH_SetLatency(FLASH_Latency_1);
    
    /* HCLK = SYSCLK */
    RCC_HCLKConfig(RCC_SYSCLK_Div1); 
    
    /* PCLK = HCLK */
    RCC_PCLKConfig(RCC_HCLK_Div1);
        
    /*  PLL configuration:  = HSE *  6 = 48 MHz */
    RCC_PREDIV1Config(RCC_PREDIV1_Div1);
    RCC_PLLConfig(RCC_PLLSource_PREDIV1, RCC_CFGR_PLLMULL6);
    
    /* Enable PLL */
    RCC_PLLCmd(ENABLE);
    
    /* PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
  } 
}

/**
  * @brief  Compare two 8-bit buffers and returns the comparison result
  * @param  pBuffer: pointer to the source buffer to be compared to.
  * @param  pBuffer1: pointer to the second source buffer to be compared to the first.
  * @param  BufferLength: size of buffer to compare.
  * @retval PASSED: Comparison is OK (the two Buffers are identical)
  *         FAILED: Comparison is NOK (Buffers are different)
  */
static TestStatus Compare_bBuffer(uint8_t* pBuffer, uint8_t* pBuffer1, uint32_t BufferLength)
{
  while (BufferLength--)
  {
    if (*pBuffer != *pBuffer1)
    {
      return FAILED;
    }

    pBuffer++;
    pBuffer1++;
  }

  return PASSED;
}

/**
  * @brief  Fill Buffer with zero value
  * @param  pBuffer: pointer on the Buffer to fill.
  * @param  BufferLength: size of buffer to Fill.
  * @retval None
  */

static void Reset_bBuffer(uint8_t *pBuffer, uint32_t BufferLenght)
{
  uint32_t indextmp = 0;

  /* Put in global buffer same values */
  for ( indextmp = 0; indextmp < BufferLenght; indextmp++ )
  {
    pBuffer[indextmp] = 0;
  }
}
#endif /* I2C_SLAVE */

/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in 10 ms.
  * @retval None
  */
void Delay(__IO uint32_t nTime)
{
  TimingDelay = nTime;

  while(TimingDelay != 0);
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
