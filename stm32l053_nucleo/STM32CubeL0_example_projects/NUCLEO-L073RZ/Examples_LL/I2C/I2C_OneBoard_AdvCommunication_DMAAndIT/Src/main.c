/**
  ******************************************************************************
  * @file    Examples_LL/I2C/I2C_OneBoard_AdvCommunication_DMAAndIT/Src/main.c
  * @author  MCD Application Team
  * @brief   This example describes how to send/receive bytes over I2C IP using
  *          the STM32L0xx I2C LL API.
  *          Peripheral initialization done using LL unitary services functions.
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

#if defined(__GNUC__)
#include <stdio.h>	/*rtt*/
#include <stdlib.h>	/*rtt*/
#endif

#include "main.h"

/** @addtogroup STM32L0xx_LL_Examples
  * @{
  */

/** @addtogroup I2C_OneBoard_AdvCommunication_DMAAndIT
  * @{
  */

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
/**
  * @brief Timeout value
  */
#if (USE_TIMEOUT == 1)
#define DMA_SEND_TIMEOUT_TC_MS          5
#define I2C_SEND_TIMEOUT_TC_MS          2
#define I2C_SEND_TIMEOUT_STOP_MS        5
#endif /* USE_TIMEOUT */

/**
  * @brief I2C devices settings
  */
/* Timing register value is computed with the STM32CubeMX Tool,
  * Fast Mode @400kHz with I2CCLK = 16 MHz,
  * rise time = 100ns, fall time = 10ns
  * Timing Value = (uint32_t)0x00300619
  */
#define I2C_TIMING                      0x00300619

#define SLAVE_CHIP_NAME                 0
#define SLAVE_CHIP_REVISION             1

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
#if (USE_TIMEOUT == 1)
uint32_t Timeout                        = 0; /* Variable used for Timeout management */
#endif /* USE_TIMEOUT */
__IO uint8_t  ubButtonPress             = 0;

/**
  * @brief Variables related to Master process
  */
/* aCommandCode declaration array    */
/* [CommandCode][RequestSlaveAnswer] */
/* {CODE, YES/NO}                    */
const char*  aCommandCode[4][4]         = {
                                           {"CHIP NAME", "YES"},
                                           {"CHIP REVISION", "YES"},
                                           {"LOW POWER", "NO"},
                                           {"WAKE UP", "NO"}
                                          };

uint32_t*     pMasterTransmitBuffer     = (uint32_t*)(&aCommandCode[0][0]);
uint8_t       ubMasterNbCommandCode     = sizeof(aCommandCode[0][0]);
uint8_t       aMasterReceiveBuffer[0xF] = {0};
__IO uint8_t  ubMasterNbDataToReceive   = sizeof(aMasterReceiveBuffer);
__IO uint8_t  ubMasterNbDataToTransmit  = 0;
uint8_t       ubMasterCommandIndex      = 0;
__IO uint8_t  ubMasterReceiveIndex      = 0;
__IO uint8_t  ubMasterTransferComplete  = 0;

/**
  * @brief Variables related to Slave process
  */
const char*   aSlaveInfo[]              = {
                                          "STM32L073RZ",
                                          "1.2.3"};

uint8_t       aSlaveReceiveBuffer[0xF]  = {0};
uint8_t*      pSlaveTransmitBuffer      = 0;
uint8_t       ubSlaveInfoIndex          = 0xFF;
__IO uint8_t  ubSlaveReceiveIndex       = 0;
__IO uint8_t  ubSlaveReceiveComplete    = 0;

/* Private function prototypes -----------------------------------------------*/
void     SystemClock_Config(void);
#if (USE_VCP_CONNECTION == 1)
void     Configure_USART(void);
#endif
void     Configure_DMA(void);
void     Configure_I2C_Slave(void);
void     Configure_I2C_Master(void);
uint8_t  Buffercmp8(uint8_t* pBuffer1, uint8_t* pBuffer2, uint8_t BufferLength);
void     FlushBuffer8(uint8_t* pBuffer1);
void     LED_Init(void);
void     LED_On(void);
void     LED_Off(void);
void     LED_Blinking(uint32_t Period);
void     WaitForUserButtonPress(void);
void     Handle_I2C_Master_Transmit(void);
void     Handle_I2C_Master_TransmitReceive(void);
void     UserButton_Init(void);

#if defined(__GNUC__)
extern void initialise_monitor_handles(void);	/*rtt*/
#endif

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
#if defined(__GNUC__)
  initialise_monitor_handles();	/*rtt*/
#endif

  /* Configure the system clock to 16 MHz */
  SystemClock_Config();

  /* Initialize LED2 */
  LED_Init();

  /* Set LED2 Off */
  LED_Off();

  /* Initialize User push-button in EXTI mode */
  UserButton_Init();

#if (USE_VCP_CONNECTION == 1) 
  /* Configure USARTx (USART IP configuration and related GPIO initialization) */
  Configure_USART();
#endif

  /* Configure DMA1_Channel2_3 (DMA IP configuration in transfer memory to peripheral (I2C3)  */
  Configure_DMA();

  /* Configure I2C1 (I2C IP configuration in Slave mode and related GPIO initialization) */
  Configure_I2C_Slave();

  /* Configure I2C3 (I2C IP configuration in Master mode and related GPIO initialization) */
  Configure_I2C_Master();

  /* Infinite loop */
  while (1)
  {
    /* Wait for User push-button press to start transfer */
    WaitForUserButtonPress();

    /* Clear User push-button related variable */
    ubButtonPress = 0;

    if(strncmp(aCommandCode[ubMasterCommandIndex][1], "NO", 2) == 0)
    {
      /* Handle I2C3 events (Master Transmit only) */
      Handle_I2C_Master_Transmit();
    }
    else
    {
      /* Handle I2C3 events (Master Transmit then Receive) */
      Handle_I2C_Master_TransmitReceive();
    }
    
    /* Prepare Index to send next command code */
    ubMasterCommandIndex++;
    if(ubMasterCommandIndex >= ubMasterNbCommandCode)
    {
      ubMasterCommandIndex = 0;
    }
  }
}

#if (USE_VCP_CONNECTION == 1) 
/**
  * @brief  This function configures USARTx Instance.
  * @note   This function is used to :
  *         -1- Enable GPIO clock and configures the USART pins.
  *         -2- Enable the USART peripheral clock and clock source.
  *         -3- Configure USART functional parameters.
  *         -4- Enable USART.
  * @note   Peripheral configuration is minimal configuration from reset values.
  *         Thus, some useless LL unitary functions calls below are provided as
  *         commented examples - setting is default configuration from reset.
  * @param  None
  * @retval None
  */
void Configure_USART(void)
{

  /* (1) Enable GPIO clock and configures the USART pins *********************/

  /* Enable the peripheral clock of GPIO Port */
  USARTx_GPIO_CLK_ENABLE();

  /* Configure Tx Pin as : Alternate function, High Speed, Push pull, Pull up */
  LL_GPIO_SetPinMode(USARTx_TX_GPIO_PORT, USARTx_TX_PIN, LL_GPIO_MODE_ALTERNATE);
  USARTx_SET_TX_GPIO_AF();
  LL_GPIO_SetPinSpeed(USARTx_TX_GPIO_PORT, USARTx_TX_PIN, LL_GPIO_SPEED_FREQ_HIGH);
  LL_GPIO_SetPinOutputType(USARTx_TX_GPIO_PORT, USARTx_TX_PIN, LL_GPIO_OUTPUT_PUSHPULL);
  LL_GPIO_SetPinPull(USARTx_TX_GPIO_PORT, USARTx_TX_PIN, LL_GPIO_PULL_UP);

  /* Configure Rx Pin as : Alternate function, High Speed, Push pull, Pull up */
  LL_GPIO_SetPinMode(USARTx_RX_GPIO_PORT, USARTx_RX_PIN, LL_GPIO_MODE_ALTERNATE);
  USARTx_SET_RX_GPIO_AF();
  LL_GPIO_SetPinSpeed(USARTx_RX_GPIO_PORT, USARTx_RX_PIN, LL_GPIO_SPEED_FREQ_HIGH);
  LL_GPIO_SetPinOutputType(USARTx_RX_GPIO_PORT, USARTx_RX_PIN, LL_GPIO_OUTPUT_PUSHPULL);
  LL_GPIO_SetPinPull(USARTx_RX_GPIO_PORT, USARTx_RX_PIN, LL_GPIO_PULL_UP);

  /* (2) Enable USART peripheral clock and clock source ***********************/
  USARTx_CLK_ENABLE();

  /* Set clock source */
  USARTx_CLK_SOURCE();

  /* (3) Configure USART functional parameters ********************************/
  
  /* Disable USART prior modifying configuration registers */
  /* Note: Commented as corresponding to Reset value */
  // LL_USART_Disable(USARTx_INSTANCE);

  /* TX/RX direction */
  LL_USART_SetTransferDirection(USARTx_INSTANCE, LL_USART_DIRECTION_TX_RX);

  /* 8 data bit, 1 start bit, 1 stop bit, no parity */
  LL_USART_ConfigCharacter(USARTx_INSTANCE, LL_USART_DATAWIDTH_8B, LL_USART_PARITY_NONE, LL_USART_STOPBITS_1);

  /* No Hardware Flow control */
  /* Reset value is LL_USART_HWCONTROL_NONE */
  // LL_USART_SetHWFlowCtrl(USARTx_INSTANCE, LL_USART_HWCONTROL_NONE);

  /* Oversampling by 16 */
  /* Reset value is LL_USART_OVERSAMPLING_16 */
  // LL_USART_SetOverSampling(USARTx_INSTANCE, LL_USART_OVERSAMPLING_16);

  /* Set Baudrate to 115200 using APB frequency set to 48000000 Hz */
  /* Frequency available for USART peripheral can also be calculated through LL RCC macro */
  /* Ex :
      Periphclk = LL_RCC_GetUSARTClockFreq(Instance); or LL_RCC_GetUARTClockFreq(Instance); depending on USART/UART instance
  
      In this example, Peripheral Clock is expected to be equal to 48000000 Hz => equal to SystemCoreClock
  */
  LL_USART_SetBaudRate(USARTx_INSTANCE, SystemCoreClock, LL_USART_OVERSAMPLING_16, 115200); 

  /* (4) Enable USART *********************************************************/
  LL_USART_Enable(USARTx_INSTANCE);
  
  /* Polling USART initialisation */
  while((!(LL_USART_IsActiveFlag_TEACK(USARTx_INSTANCE))) || (!(LL_USART_IsActiveFlag_REACK(USARTx_INSTANCE))))
  { 
  }
}

/**
  * @brief  Retargets the C library printf function to the USARTx.
  * @param  ch: character to send
  * @param  f: pointer to file (not used)
  * @retval The character transmitted
  */
#ifdef __GNUC__
/* With GCC, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
int __io_putchar(int ch)
#else
int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the EVAL_COM1 and Loop until the end of transmission */

  /* Wait for TXE flag to be raised */
  while (!LL_USART_IsActiveFlag_TXE(USARTx_INSTANCE))
  {
  }

  /* Write character in Transmit Data register.
     TXE flag is cleared by writing data in TDR register */
  LL_USART_TransmitData8(USARTx_INSTANCE, ch);

  /* Wait for TC flag to be raised for last char */
  while (!LL_USART_IsActiveFlag_TC(USARTx_INSTANCE))
  {
  }
  
  return ch;
}
#endif

/**
  * @brief  This function configures the DMA Channels for I2C3(TXDR) and I2C3(RXDR).
  * @note   This function is used to :
  *         -1- Enable the clock of DMA1.
  *         -2- Configure NVIC for DMA1_Channel2_3 and DMA1_Channel2_3.
  *         -3- Configure the DMA functional parameters for Master Transmit.
  *         -4- Configure the DMA functional parameters for Master Receive.
  *         -5- Enable DMA1 interrupts complete/error.
  * @param   None
  * @retval  None
  */
void Configure_DMA(void)
{
  /* (1) Enable the clock of DMA1 */
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);

  /* (2) Configure NVIC for DMA1_Channel2_3 */
  NVIC_SetPriority(DMA1_Channel2_3_IRQn, 0x4);
  NVIC_EnableIRQ(DMA1_Channel2_3_IRQn);

  /* (3) Configure the DMA functional parameters for Master Transmit */
  LL_DMA_ConfigTransfer(DMA1, LL_DMA_CHANNEL_2, LL_DMA_DIRECTION_MEMORY_TO_PERIPH | \
                                                LL_DMA_PRIORITY_HIGH              | \
                                                LL_DMA_MODE_NORMAL                | \
                                                LL_DMA_PERIPH_NOINCREMENT         | \
                                                LL_DMA_MEMORY_INCREMENT           | \
                                                LL_DMA_PDATAALIGN_BYTE            | \
                                                LL_DMA_MDATAALIGN_BYTE);
  LL_DMA_ConfigAddresses(DMA1, LL_DMA_CHANNEL_2, (uint32_t)(*pMasterTransmitBuffer), (uint32_t)LL_I2C_DMA_GetRegAddr(I2C3, LL_I2C_DMA_REG_DATA_TRANSMIT), LL_DMA_GetDataTransferDirection(DMA1, LL_DMA_CHANNEL_2));
  LL_DMA_SetPeriphRequest(DMA1, LL_DMA_CHANNEL_2, LL_DMA_REQUEST_14);
  
  /* (4) Configure the DMA functional parameters for Master Receive */
  LL_DMA_ConfigTransfer(DMA1, LL_DMA_CHANNEL_3, LL_DMA_DIRECTION_PERIPH_TO_MEMORY | \
                                                LL_DMA_PRIORITY_HIGH              | \
                                                LL_DMA_MODE_NORMAL                | \
                                                LL_DMA_PERIPH_NOINCREMENT         | \
                                                LL_DMA_MEMORY_INCREMENT           | \
                                                LL_DMA_PDATAALIGN_BYTE            | \
                                                LL_DMA_MDATAALIGN_BYTE);
  LL_DMA_ConfigAddresses(DMA1, LL_DMA_CHANNEL_3, (uint32_t)LL_I2C_DMA_GetRegAddr(I2C3, LL_I2C_DMA_REG_DATA_RECEIVE), (uint32_t)&(aMasterReceiveBuffer), LL_DMA_GetDataTransferDirection(DMA1, LL_DMA_CHANNEL_3));
  LL_DMA_SetPeriphRequest(DMA1, LL_DMA_CHANNEL_3, LL_DMA_REQUEST_14);
  
  /* (5) Enable DMA1 interrupts complete/error */
  LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_2);
  LL_DMA_EnableIT_TE(DMA1, LL_DMA_CHANNEL_2);
  LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_3);
  LL_DMA_EnableIT_TE(DMA1, LL_DMA_CHANNEL_3);
}

/**
  * @brief  This function configures I2C1 in Slave mode.
  * @note   This function is used to :
  *         -1- Enables GPIO clock and configures the I2C1 pins.
  *         -2- Enable the I2C1 peripheral clock and I2C1 clock source.
  *         -3- Configure NVIC for I2C1.
  *         -4- Configure I2C1 functional parameters.
  *         -5- Enable I2C1.
  *         -6- Enable I2C1 address match/error interrupts.
  * @note   Peripheral configuration is minimal configuration from reset values.
  *         Thus, some useless LL unitary functions calls below are provided as
  *         commented examples - setting is default configuration from reset.
  * @param  None
  * @retval None
  */
void Configure_I2C_Slave(void)
{
  uint32_t timing = 0;

  /* (1) Enables GPIO clock and configures the I2C1 pins **********************/
  /*    (SCL on PB.8, SDA on PB.9)                     **********************/

  /* Enable the peripheral clock of GPIOB */
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOB);

  /* Configure SCL Pin as : Alternate function, High Speed, Open drain, Pull up */
  LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_8, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetAFPin_8_15(GPIOB, LL_GPIO_PIN_8, LL_GPIO_AF_4);
  LL_GPIO_SetPinSpeed(GPIOB, LL_GPIO_PIN_8, LL_GPIO_SPEED_FREQ_HIGH);
  LL_GPIO_SetPinOutputType(GPIOB, LL_GPIO_PIN_8, LL_GPIO_OUTPUT_OPENDRAIN);
  LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_8, LL_GPIO_PULL_UP);

  /* Configure SDA Pin as : Alternate function, High Speed, Open drain, Pull up */
  LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_9, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetAFPin_8_15(GPIOB, LL_GPIO_PIN_9, LL_GPIO_AF_4);
  LL_GPIO_SetPinSpeed(GPIOB, LL_GPIO_PIN_9, LL_GPIO_SPEED_FREQ_HIGH);
  LL_GPIO_SetPinOutputType(GPIOB, LL_GPIO_PIN_9, LL_GPIO_OUTPUT_OPENDRAIN);
  LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_9, LL_GPIO_PULL_UP);

  /* (2) Enable the I2C1 peripheral clock and I2C1 clock source ***************/

  /* Enable the peripheral clock for I2C1 */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_I2C1);

  /* Set I2C1 clock source as SYSCLK */
  LL_RCC_SetI2CClockSource(LL_RCC_I2C1_CLKSOURCE_SYSCLK);

  /* (3) Configure NVIC for I2C1 **********************************************/

  /* Configure Event and Error IT:
   *  - Set priority for I2C1_IRQn
   *  - Enable I2C1_IRQn
   */
  NVIC_SetPriority(I2C1_IRQn, 0xF);
  NVIC_EnableIRQ(I2C1_IRQn);

  /* (4) Configure I2C1 functional parameters *********************************/

  /* Disable I2C1 prior modifying configuration registers */
  LL_I2C_Disable(I2C1);

  /* Configure the SDA setup, hold time and the SCL high, low period */
  /* Timing register value is computed with the STM32CubeMX Tool,
    * Fast Mode @400kHz with I2CCLK = 16 MHz,
    * rise time = 100ns, fall time = 10ns
    * Timing Value = (uint32_t)0x00300619
    */
  timing = __LL_I2C_CONVERT_TIMINGS(0x0, 0x3, 0x0, 0x06, 0x19);
  LL_I2C_SetTiming(I2C1, timing);

  /* Configure the Own Address1 :
   *  - OwnAddress1 is SLAVE_OWN_ADDRESS
   *  - OwnAddrSize is LL_I2C_OWNADDRESS1_7BIT
   *  - Own Address1 is enabled
   */
  LL_I2C_SetOwnAddress1(I2C1, SLAVE_OWN_ADDRESS, LL_I2C_OWNADDRESS1_7BIT);
  LL_I2C_EnableOwnAddress1(I2C1);

  /* Enable Clock stretching */
  /* Reset Value is Clock stretching enabled */
  //LL_I2C_EnableClockStretching(I2C1);

  /* Configure Digital Noise Filter */
  /* Reset Value is 0x00            */
  //LL_I2C_SetDigitalFilter(I2C1, 0x00);

  /* Enable Analog Noise Filter           */
  /* Reset Value is Analog Filter enabled */
  //LL_I2C_EnableAnalogFilter(I2C1);

  /* Enable General Call                  */
  /* Reset Value is General Call disabled */
  //LL_I2C_EnableGeneralCall(I2C1);

  /* Configure the 7bits Own Address2               */
  /* Reset Values of :
   *     - OwnAddress2 is 0x00
   *     - OwnAddrMask is LL_I2C_OWNADDRESS2_NOMASK
   *     - Own Address2 is disabled
   */
  //LL_I2C_SetOwnAddress2(I2C1, 0x00, LL_I2C_OWNADDRESS2_NOMASK);
  //LL_I2C_DisableOwnAddress2(I2C1);

  /* Enable Peripheral in I2C mode */
  /* Reset Value is I2C mode */
  //LL_I2C_SetMode(I2C1, LL_I2C_MODE_I2C);

  /* (5) Enable I2C1 **********************************************************/
  LL_I2C_Enable(I2C1);

  /* (6) Enable I2C1 address match/error interrupts:
   *  - Enable Address Match Interrupt
   *  - Enable Not acknowledge received interrupt
   *  - Enable Error interrupts
   *  - Enable Stop interrupt
   */
  LL_I2C_EnableIT_ADDR(I2C1);
  LL_I2C_EnableIT_NACK(I2C1);
  LL_I2C_EnableIT_ERR(I2C1);
  LL_I2C_EnableIT_STOP(I2C1);
}

/**
  * @brief  This function configures I2C3 in Master mode.
  * @note   This function is used to :
  *         -1- Enables GPIO clock and configures the I2C3 pins.
  *         -2- Enable the I2C3 peripheral clock and I2C3 clock source.
  *         -3- Configure I2C3 functional parameters.
  *         -4- Enable DMA transmission requests and I2C3.
  * @note   Peripheral configuration is minimal configuration from reset values.
  *         Thus, some useless LL unitary functions calls below are provided as
  *         commented examples - setting is default configuration from reset.
  * @param  None
  * @retval None
  */
void Configure_I2C_Master(void)
{
  /* (1) Enables GPIO clock and configures the I2C3 pins **********************/
  /*    (SCL on PC.0, SDA on PC.1)                     **********************/

  /* Enable the peripheral clock of GPIOC */
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOC);

  /* Configure SCL Pin as : Alternate function, High Speed, Open drain, Pull up */
  LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_0, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetAFPin_0_7(GPIOC, LL_GPIO_PIN_0, LL_GPIO_AF_7);
  LL_GPIO_SetPinSpeed(GPIOC, LL_GPIO_PIN_0, LL_GPIO_SPEED_FREQ_HIGH);
  LL_GPIO_SetPinOutputType(GPIOC, LL_GPIO_PIN_0, LL_GPIO_OUTPUT_OPENDRAIN);
  LL_GPIO_SetPinPull(GPIOC, LL_GPIO_PIN_0, LL_GPIO_PULL_UP);

  /* Configure SDA Pin as : Alternate function, High Speed, Open drain, Pull up */
  LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_1, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetAFPin_0_7(GPIOC, LL_GPIO_PIN_1, LL_GPIO_AF_7);
  LL_GPIO_SetPinSpeed(GPIOC, LL_GPIO_PIN_1, LL_GPIO_SPEED_FREQ_HIGH);
  LL_GPIO_SetPinOutputType(GPIOC, LL_GPIO_PIN_1, LL_GPIO_OUTPUT_OPENDRAIN);
  LL_GPIO_SetPinPull(GPIOC, LL_GPIO_PIN_1, LL_GPIO_PULL_UP);

  /* (2) Enable the I2C3 peripheral clock and I2C3 clock source ***************/

  /* Enable the peripheral clock for I2C3 */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_I2C3);

  /* Set I2C3 clock source as SYSCLK */
  LL_RCC_SetI2CClockSource(LL_RCC_I2C3_CLKSOURCE_SYSCLK);

  /* (3) Configure I2C3 functional parameters ********************************/
  
  /* Disable I2C3 prior modifying configuration registers */
  LL_I2C_Disable(I2C3);

  /* Configure the SDA setup, hold time and the SCL high, low period */
  /* (uint32_t)0x00300619 = I2C_TIMING*/
  LL_I2C_SetTiming(I2C3, I2C_TIMING);

  /* Configure the Own Address1                   */
  /* Reset Values of :
   *     - OwnAddress1 is 0x00
   *     - OwnAddrSize is LL_I2C_OWNADDRESS1_7BIT
   *     - Own Address1 is disabled
   */
  //LL_I2C_SetOwnAddress1(I2C3, 0x00, LL_I2C_OWNADDRESS1_7BIT);
  //LL_I2C_DisableOwnAddress1(I2C3);

  /* Enable Clock stretching */
  /* Reset Value is Clock stretching enabled */
  //LL_I2C_EnableClockStretching(I2C3);

  /* Configure Digital Noise Filter */
  /* Reset Value is 0x00            */
  //LL_I2C_SetDigitalFilter(I2C3, 0x00);

  /* Enable Analog Noise Filter           */
  /* Reset Value is Analog Filter enabled */
  //LL_I2C_EnableAnalogFilter(I2C3);

  /* Enable General Call                  */
  /* Reset Value is General Call disabled */
  //LL_I2C_EnableGeneralCall(I2C3);

  /* Configure the 7bits Own Address2               */
  /* Reset Values of :
   *     - OwnAddress2 is 0x00
   *     - OwnAddrMask is LL_I2C_OWNADDRESS2_NOMASK
   *     - Own Address2 is disabled
   */
  //LL_I2C_SetOwnAddress2(I2C3, 0x00, LL_I2C_OWNADDRESS2_NOMASK);
  //LL_I2C_DisableOwnAddress2(I2C3);

  /* Configure the Master to operate in 7-bit or 10-bit addressing mode */
  /* Reset Value is LL_I2C_ADDRESSING_MODE_7BIT                         */
  //LL_I2C_SetMasterAddressingMode(I2C3, LL_I2C_ADDRESSING_MODE_7BIT);

  /* Enable Peripheral in I2C mode */
  /* Reset Value is I2C mode */
  //LL_I2C_SetMode(I2C3, LL_I2C_MODE_I2C);

  /* (4) Enable DMA transmission requests and I2C3 ****************************/
  LL_I2C_EnableDMAReq_RX(I2C3);
  LL_I2C_EnableDMAReq_TX(I2C3);
  LL_I2C_Enable(I2C3);
}

/**
  * @brief  Flush 8-bit buffer.
  * @param  pBuffer1: pointer to the buffer to be flushed.
  * @retval None
  */
void FlushBuffer8(uint8_t* pBuffer1)
{
  uint8_t Index = 0;

  for (Index = 0; Index < sizeof(pBuffer1); Index++)
  {
    pBuffer1[Index] = 0;
  }
}

/**
  * @brief  Compares two 8-bit buffers and returns the comparison result.
  * @param  pBuffer1: pointer to the source buffer to be compared to.
  * @param  pBuffer2: pointer to the second source buffer to be compared to the first.
  * @param  BufferLength: buffer's length.
  * @retval 0: Comparison is OK (the two Buffers are identical)
  *         Value different from 0: Comparison is NOK (Buffers are different)
  */
uint8_t Buffercmp8(uint8_t* pBuffer1, uint8_t* pBuffer2, uint8_t BufferLength)
{
  while (BufferLength--)
  {
    if (*pBuffer1 != *pBuffer2)
    {
      return 1;
    }

    pBuffer1++;
    pBuffer2++;
  }

  return 0;
}

/**
  * @brief  Initialize LED2.
  * @param  None
  * @retval None
  */
void LED_Init(void)
{
  /* Enable the LED2 Clock */
  LED2_GPIO_CLK_ENABLE();

  /* Configure IO in output push-pull mode to drive external LED2 */
  LL_GPIO_SetPinMode(LED2_GPIO_PORT, LED2_PIN, LL_GPIO_MODE_OUTPUT);
  /* Reset value is LL_GPIO_OUTPUT_PUSHPULL */
  //LL_GPIO_SetPinOutputType(LED2_GPIO_PORT, LED2_PIN, LL_GPIO_OUTPUT_PUSHPULL);
  /* Reset value is LL_GPIO_SPEED_FREQ_LOW */
  //LL_GPIO_SetPinSpeed(LED2_GPIO_PORT, LED2_PIN, LL_GPIO_SPEED_FREQ_LOW);
  /* Reset value is LL_GPIO_PULL_NO */
  //LL_GPIO_SetPinPull(LED2_GPIO_PORT, LED2_PIN, LL_GPIO_PULL_NO);
}

/**
  * @brief  Turn-on LED2.
  * @param  None
  * @retval None
  */
void LED_On(void)
{
  /* Turn LED2 on */
  LL_GPIO_SetOutputPin(LED2_GPIO_PORT, LED2_PIN);
}

/**
  * @brief  Turn-off LED2.
  * @param  None
  * @retval None
  */
void LED_Off(void)
{
  /* Turn LED2 off */
  LL_GPIO_ResetOutputPin(LED2_GPIO_PORT, LED2_PIN);
}

/**
  * @brief  Set LED2 to Blinking mode for an infinite loop (toggle period based on value provided as input parameter).
  * @param  Period : Period of time (in ms) between each toggling of LED
  *   This parameter can be user defined values. Pre-defined values used in that example are :   
  *     @arg LED_BLINK_FAST : Fast Blinking
  *     @arg LED_BLINK_SLOW : Slow Blinking
  *     @arg LED_BLINK_ERROR : Error specific Blinking
  * @retval None
  */
void LED_Blinking(uint32_t Period)
{
  /* Turn LED2 on */
  LL_GPIO_SetOutputPin(LED2_GPIO_PORT, LED2_PIN);

  /* Toggle IO in an infinite loop */
  while (1)
  {
    LL_GPIO_TogglePin(LED2_GPIO_PORT, LED2_PIN);  
    LL_mDelay(Period);
  }
}

/**
  * @brief  Configures User push-button in GPIO or EXTI Line Mode.
  * @param  None 
  * @retval None
  */
void UserButton_Init(void)
{
  /* Enable the BUTTON Clock */
  USER_BUTTON_GPIO_CLK_ENABLE();

  /* Configure GPIO for BUTTON */
  LL_GPIO_SetPinMode(USER_BUTTON_GPIO_PORT, USER_BUTTON_PIN, LL_GPIO_MODE_INPUT);
  LL_GPIO_SetPinPull(USER_BUTTON_GPIO_PORT, USER_BUTTON_PIN, LL_GPIO_PULL_NO);

  /* Connect External Line to the GPIO*/
  USER_BUTTON_SYSCFG_SET_EXTI();

  /* Enable a rising trigger External line 13 Interrupt */
  USER_BUTTON_EXTI_LINE_ENABLE();
  USER_BUTTON_EXTI_FALLING_TRIG_ENABLE();

  /* Configure NVIC for USER_BUTTON_EXTI_IRQn */
  NVIC_EnableIRQ(USER_BUTTON_EXTI_IRQn); 
  NVIC_SetPriority(USER_BUTTON_EXTI_IRQn, 0x03);  
}

/**
  * @brief  Wait for User push-button press to start transfer.
  * @param  None 
  * @retval None
  */
  /*  */
void WaitForUserButtonPress(void)
{
  while (ubButtonPress == 0)
  {
    LL_GPIO_TogglePin(LED2_GPIO_PORT, LED2_PIN);
    LL_mDelay(LED_BLINK_FAST);
  }
  /* Turn LED2 off */
  LL_GPIO_ResetOutputPin(LED2_GPIO_PORT, LED2_PIN);
}

/**
  * @brief  This Function handle Master events to perform a transmission process
  * @note  This function is composed in different steps :
  *        -1- Configure DMA parameters for Command Code transfer.
  *        -2- Enable DMA transfer.
  *        -3- Initiate a Start condition to the Slave device.
  *        -4- Loop until end of DMA transfer completed (DMA TC raised).
  *        -5- Loop until end of master transfer completed (STOP flag raised).
  *        -6- Clear pending flags, Data Command Code are checking into Slave process.
  * @param  None
  * @retval None
  */
void Handle_I2C_Master_Transmit(void)
{
  /* (1) Configure DMA parameters for Command Code transfer *******************/
  pMasterTransmitBuffer    = (uint32_t*)(&aCommandCode[ubMasterCommandIndex][0]);
  ubMasterNbDataToTransmit = strlen((char *)pMasterTransmitBuffer[0]);

  LL_DMA_SetMemoryAddress(DMA1, LL_DMA_CHANNEL_2, (uint32_t)(*pMasterTransmitBuffer));
  LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_2, ubMasterNbDataToTransmit);

  /* (2) Enable DMA transfer **************************************************/
  LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_2);

  /* (3) Initiate a Start condition to the Slave device ***********************/

  /* Master Generate Start condition for a write request:
   *  - to the Slave with a 7-Bit SLAVE_OWN_ADDRESS
   *  - with a auto stop condition generation when transmit all bytes
   *  - No specific answer is needed from Slave Device, configure auto-stop condition
   */
  LL_I2C_HandleTransfer(I2C3, SLAVE_OWN_ADDRESS, LL_I2C_ADDRSLAVE_7BIT, ubMasterNbDataToTransmit, LL_I2C_MODE_AUTOEND, LL_I2C_GENERATE_START_WRITE);

  /* (4) Loop until end of transfer completed (DMA TC raised) *****************/

#if (USE_TIMEOUT == 1)
  Timeout = DMA_SEND_TIMEOUT_TC_MS;
#endif /* USE_TIMEOUT */

  /* Loop until DMA transfer complete event */
  while(!ubMasterTransferComplete)
  {
#if (USE_TIMEOUT == 1)
    /* Check Systick counter flag to decrement the time-out value */
    if (LL_SYSTICK_IsActiveCounterFlag()) 
    {
      if(Timeout-- == 0)
      {
        /* Time-out occurred. Set LED to blinking mode */
        LED_Blinking(LED_BLINK_SLOW);
      }
    }
#endif /* USE_TIMEOUT */
  }

  /* (5) Loop until end of master process completed (STOP flag raised) ********/
#if (USE_TIMEOUT == 1)
  Timeout = I2C_SEND_TIMEOUT_STOP_MS;
#endif /* USE_TIMEOUT */

  /* Loop until STOP flag is raised  */
  while(!LL_I2C_IsActiveFlag_STOP(I2C3))
  {
#if (USE_TIMEOUT == 1)
    /* Check Systick counter flag to decrement the time-out value */
    if (LL_SYSTICK_IsActiveCounterFlag()) 
    {
      if(Timeout-- == 0)
      {
        /* Time-out occurred. Set LED2 to blinking mode */
        LED_Blinking(LED_BLINK_SLOW);
      }
    }
#endif /* USE_TIMEOUT */
  }

  /* (6) Clear pending flags, Data Command Code are checking into Slave process */
  /* End of Master Process */
  LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_2);
  LL_I2C_ClearFlag_STOP(I2C3);

  /* Display through external Terminal IO the Slave Answer received */
  printf("%s : %s\n\r", (char*)(aCommandCode[ubMasterCommandIndex][0]), (char*)aMasterReceiveBuffer);

  /* Turn LED2 On */
  /* Master sequence completed successfully*/
  LED_On();
  /* Keep LED2 On, 500 MilliSeconds */
  LL_mDelay(500);
  LED_Off();

  /* Clear and Reset process variables and arrays */
  ubMasterTransferComplete = 0;
  ubMasterNbDataToTransmit = 0;
  ubMasterReceiveIndex     = 0;
  FlushBuffer8(aMasterReceiveBuffer);
}

/**
  * @brief  This Function handle Master events to perform a transmission then a reception process
  * @note   This function is composed in different steps :
  *         -1- Configure DMA parameters for Command Code transfer.
  *         -2- Enable DMA transfer.
  *         -3- Initiate a Start condition to the Slave device.
  *         -4- Loop until end of DMA transfer completed (DMA TC raised).
  *         -5- Loop until end of master transfer completed (TC flag raised).
  *         -6- Configure DMA to receive data from slave.
  *         -7- Initiate a ReStart condition to the Slave device.
  *         -8- Loop until end of master process completed (STOP flag raised).
  *         -9- Clear pending flags, Data Command Code are checking into Slave process.
  * @param  None
  * @retval None
  */
void Handle_I2C_Master_TransmitReceive(void)
{
  /* (1) Configure DMA parameters for Command Code transfer *******************/
  pMasterTransmitBuffer    = (uint32_t*)(&aCommandCode[ubMasterCommandIndex][0]);
  ubMasterNbDataToTransmit = strlen((char *)pMasterTransmitBuffer[0]);

  LL_DMA_SetMemoryAddress(DMA1, LL_DMA_CHANNEL_2, (uint32_t)(*pMasterTransmitBuffer));
  LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_2, ubMasterNbDataToTransmit);

  /* (2) Enable DMA transfer **************************************************/
  LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_2);

  /* (3) Initiate a Start condition to the Slave device ***********************/

  /* Master Generate Start condition for a write request:
   *  - to the Slave with a 7-Bit SLAVE_OWN_ADDRESS
   *  - with a no stop condition generation when transmit all bytes
   *  - A specific answer is needed from Slave Device, configure no-stop condition
   */
  LL_I2C_HandleTransfer(I2C3, SLAVE_OWN_ADDRESS, LL_I2C_ADDRSLAVE_7BIT, ubMasterNbDataToTransmit, LL_I2C_MODE_SOFTEND, LL_I2C_GENERATE_START_WRITE);

  /* (4) Loop until end of transfer completed (DMA TC raised) *****************/

#if (USE_TIMEOUT == 1)
  Timeout = DMA_SEND_TIMEOUT_TC_MS;
#endif /* USE_TIMEOUT */

  /* Loop until DMA transfer complete event */
  while(!ubMasterTransferComplete)
  {
#if (USE_TIMEOUT == 1)
    /* Check Systick counter flag to decrement the time-out value */
    if (LL_SYSTICK_IsActiveCounterFlag()) 
    {
      if(Timeout-- == 0)
      {
        /* Time-out occurred. Set LED to blinking mode */
        LED_Blinking(LED_BLINK_SLOW);
      }
    }
#endif /* USE_TIMEOUT */
  }

  /* Reset ubMasterTransferComplete flag */
  ubMasterTransferComplete = 0;

  /* (5) Loop until end of master transfer completed (TC flag raised) *********/
  /* Wait Master Transfer completed */
#if (USE_TIMEOUT == 1)
  Timeout = I2C_SEND_TIMEOUT_TC_MS;
#endif /* USE_TIMEOUT */

  while(LL_I2C_IsActiveFlag_TC(I2C3) != 1)
  {
#if (USE_TIMEOUT == 1)
    /* Check Systick counter flag to decrement the time-out value */
    if (LL_SYSTICK_IsActiveCounterFlag()) 
    {
      if(Timeout-- == 0)
      {
        /* Time-out occurred. Set LED to blinking mode */
        LED_Blinking(LED_BLINK_SLOW);
      }
    }
#endif /* USE_TIMEOUT */
  }

  /* (6) Configure DMA to receive data from slave *****************************/
  LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_2);
  LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_3, ubMasterNbDataToReceive);
  LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_3);

  /* (7) Initiate a ReStart condition to the Slave device *********************/
  /* Master Generate Start condition for a write request:
   *    - to the Slave with a 7-Bit SLAVE_OWN_ADDRESS
   *    - with a auto stop condition generation when transmit all bytes
   */
  LL_I2C_HandleTransfer(I2C3, SLAVE_OWN_ADDRESS, LL_I2C_ADDRSLAVE_7BIT, ubMasterNbDataToReceive, LL_I2C_MODE_AUTOEND, LL_I2C_GENERATE_RESTART_7BIT_READ);

  /* (8) Loop until end of master process completed (STOP flag raised) ********/
#if (USE_TIMEOUT == 1)
  Timeout = I2C_SEND_TIMEOUT_STOP_MS;
#endif /* USE_TIMEOUT */

  /* Loop until STOP flag is raised  */
  while(!LL_I2C_IsActiveFlag_STOP(I2C3))
  {
#if (USE_TIMEOUT == 1)
    /* Check Systick counter flag to decrement the time-out value */
    if (LL_SYSTICK_IsActiveCounterFlag()) 
    {
      if(Timeout-- == 0)
      {
        /* Time-out occurred. Set LED2 to blinking mode */
        LED_Blinking(LED_BLINK_SLOW);
      }
    }
#endif /* USE_TIMEOUT */
  }

  /* (9) Clear pending flags, Data Command Code are checking into Slave process */
  /* End of Master Process */
  LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_3);
  LL_I2C_ClearFlag_STOP(I2C3);

  /* Display through external Terminal IO the Slave Answer received */
  printf("%s : %s\n\r", (char*)(aCommandCode[ubMasterCommandIndex][0]), (char*)aMasterReceiveBuffer);

  /* Turn LED2 On */
  /* Master sequence completed successfully*/
  LED_On();
  /* Keep LED2 On, 500 MilliSeconds */
  LL_mDelay(500);
  LED_Off();

  /* Clear and Reset process variables and arrays */
  ubMasterTransferComplete = 0;
  ubMasterNbDataToTransmit = 0;
  ubMasterReceiveIndex     = 0;
  FlushBuffer8(aMasterReceiveBuffer);
}

/**
  * @brief  System Clock Configuration
      *         The system Clock is configured as follows :
      *            System Clock source            = HSI RC
      *            SYSCLK(Hz)                     = 16000000
      *            HCLK(Hz)                       = 16000000
      *            AHB Prescaler                  = 1
      *            APB1 Prescaler                 = 1
      *            APB2 Prescaler                 = 1
      *            HSI Frequency(Hz)              = 16000000
      *            Flash Latency(WS)              = 0
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
    LL_RCC_PLL_Disable();
    /* Set new latency */
    LL_FLASH_SetLatency(LL_FLASH_LATENCY_0);
 
    /* HSI configuration and activation */
    LL_RCC_HSI_Enable();
    LL_RCC_HSI_DisableDivider();
    while(LL_RCC_HSI_IsReady() != 1) 
    {
    };
    
  /* Sysclk activation on the HSI */
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSI);
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSI) 
  {
  };
  
  /* Set AHB & APB1 & APB2 prescaler*/
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);

  /* Disable MSI */
  LL_RCC_MSI_Disable();
  while(LL_RCC_MSI_IsReady() != 0) 
  {
  };

  /* Set systick to 1ms in using frequency set to 16MHz */
  LL_Init1msTick(16000000);
  
  /* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
  LL_SetSystemCoreClock(16000000);
}
/******************************************************************************/
/*   IRQ HANDLER TREATMENT Functions                                          */
/******************************************************************************/
/**
  * @brief  Function to manage User push-button
  * @param  None
  * @retval None
  */
void UserButton_Callback(void)
{
  /* Update User push-button variable : to be checked in waiting loop in main program */
  ubButtonPress = 1;
}

/**
  * @brief  Function called from I2C IRQ Handler when TXIS flag is set
  *         Function is in charge of transmit a byte on I2C lines.
  * @param  None
  * @retval None
  */
void Slave_Ready_To_Transmit_Callback(void)
{
  /* Send the Byte requested by the Master */
  LL_I2C_TransmitData8(I2C1, (uint8_t)(*pSlaveTransmitBuffer++));
}

/**
  * @brief  Function called from I2C IRQ Handler when RXNE flag is set
  *         Function is in charge of retrieving received byte on I2C lines.
  * @param  None
  * @retval None
  */
void Slave_Reception_Callback(void)
{
  /* Read character in Receive Data register.
  RXNE flag is cleared by reading data in RXDR register */
  aSlaveReceiveBuffer[ubSlaveReceiveIndex++] = LL_I2C_ReceiveData8(I2C1);

  /* Check Command code */
  if(Buffercmp8((uint8_t*)aSlaveReceiveBuffer, (uint8_t*)(aCommandCode[0][0]), (ubSlaveReceiveIndex-1)) == 0)
  {
    ubSlaveInfoIndex = SLAVE_CHIP_NAME;
    pSlaveTransmitBuffer = (uint8_t*)(aSlaveInfo[ubSlaveInfoIndex]);
  }
  else if(Buffercmp8((uint8_t*)aSlaveReceiveBuffer, (uint8_t*)(aCommandCode[1][0]), (ubSlaveReceiveIndex-1)) == 0)
  {
    ubSlaveInfoIndex = SLAVE_CHIP_REVISION;
    pSlaveTransmitBuffer = (uint8_t*)(aSlaveInfo[ubSlaveInfoIndex]);
  }
}

/**
  * @brief  Function called from I2C IRQ Handler when STOP flag is set
  *         Function is in charge of checking data received,
  *         LED2 is On if data are correct.
  * @param  None
  * @retval None
  */
void Slave_Complete_Callback(void)
{
  /* Clear and Reset process variables and arrays */
  ubSlaveReceiveIndex       = 0;
  ubSlaveReceiveComplete    = 0;
  FlushBuffer8(aSlaveReceiveBuffer);
}

/**
  * @brief  DMA transfer complete callback
  * @note   This function is executed when the transfer complete interrupt
  *         is generated
  * @retval None
  */
void Transfer_Complete_Callback()
{
  /* DMA transfer completed */
  ubMasterTransferComplete = 1;
}

/**
  * @brief  DMA transfer error callback
  * @note   This function is executed when the transfer error interrupt
  *         is generated during DMA transfer
  * @retval None
  */
void Transfer_Error_Callback()
{
  /* Disable DMA1_Channel2_3_IRQn */
  NVIC_DisableIRQ(DMA1_Channel2_3_IRQn);
                  
  /* Error detected during DMA transfer */
  LED_Blinking(LED_BLINK_ERROR);
}

/**
  * @brief  Function called in case of error detected in I2C IT Handler
  * @param  None
  * @retval None
  */
void Error_Callback(void)
{
  /* Disable I2C1_IRQn */
  NVIC_DisableIRQ(I2C1_IRQn);

  /* Unexpected event : Set LED2 to Blinking mode to indicate error occurs */
  LED_Blinking(LED_BLINK_ERROR);
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
     ex: printf("Wrong parameters value: file %s on line %d", file, line) */

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
