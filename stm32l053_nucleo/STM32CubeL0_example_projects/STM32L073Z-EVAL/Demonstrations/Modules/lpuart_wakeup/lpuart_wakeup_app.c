/**
  ******************************************************************************
  * @file    lpuart_wakeup_app.c
  * @author  MCD Application Team   
  * @brief   thermometer system information.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2016 STMicroelectronics International N.V. All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

#define __LPUART_WAKEUP_APP_C

/* Includes ------------------------------------------------------------------*/
#include "string.h"
#include "stdio.h"
#include "k_config.h"
#include "k_module.h"
#include "k_menu.h"
#include "k_storage.h"

/** @addtogroup LPUART_WAKEUP_MODULE
  * @{
  */

/** @defgroup SYSTEM_INFO
  * @brief system info routines 
  * @{
  */

/* Private typedef ----------------------------------------------------------*/    
/* Private constants ----------------------------------------------------------*/
#define IDD_MEASURE_TITLE_STOP            "Stop mode measure"
#define TITLE_RETURN                      "RETURN"
#define TITLE_APPLI                       "LPUART STOP Wakeup"
#define TITLE_ANYCHAR                     "Wakeup mode 1" 
#define TITLE_SPECIALCHAR                 "Wakeup mode 2"

/* Function prototypes ------------------------------------------------------*/
KMODULE_RETURN _LpUartWakeupDemoExec(void);
KMODULE_RETURN _LpUartWakeupConfig(void);
KMODULE_RETURN _LpUartWakeupUnConfig(void);

void LpUartWakeupDemo(void);
void LpUartWakeupIT1Demo(void);
void LpUartWakeupIT2Demo(void);
void LpUartWakeupMenuUserAction(uint8_t sel);

extern void SystemClock_Config(void);

/* Private Variable ----------------------------------------------------------*/
/* UART handler declaration */
UART_HandleTypeDef UartHandle;
uint8_t submenu_sel=0;

/* Buffer used for transmission */
__IO uint8_t aTxBuffer1[][275] = {
"\n\r*******************************************************************\n\r \
++ UART-Hyperterminal communication based on LP UART in IT mode ++\n\r \
\n\rPlease to Wakeup from STOP mode, Enter any character using keyboard\n\r \
at least 3 seconds after green LED (LD1) switched off:\n\r",
"\n\r*******************************************************************\n\r \
++ UART-Hyperterminal communication based on LP UART in IT mode ++\n\r \
\n\rPlease to Wakeup from STOP mode, Enter the character 'R' using keyboard\n\r \
at least 3 seconds after green LED (LD1) switched off:\n\r"};

__IO uint8_t aTxBuffer2[][70] = { " => Start bit detection: wake-up successful from STOP mode !\n\r ", \
                             " => 4-bit address match: wake-up successful from STOP mode !\n\r "};
__IO uint8_t aTxBuffer4[] = " \n\r Stop mode current measure: ";
__IO uint8_t aTxBuffer5[] = "  Idd measurement not available. ";
__IO uint8_t aTxBuffer6[] = " \n\r";
__IO uint8_t LCDStr[] = "                              ";

/* Buffer used for reception */
UART_WakeUpTypeDef WakeUpSelection; 
uint8_t aWakeUpTrigger[][10] = {"\n\r","\n\r R"}; //'R' ascii code={0x82}; adress->0x2

//---------------------
/* LpUart IT mode menu: wakeup on any character */
const tMenuItem LpUartIT1ModeMenuItems[] ={
{"", 14, 30, TYPE_EXEC, MODULE_NONE, LpUartWakeupIT1Demo, LpUartWakeupMenuUserAction, NULL, NULL },
};
const tMenu LpUartIT1ModeMenu = { "WAKEUP: mode 1", LpUartIT1ModeMenuItems, countof(LpUartIT1ModeMenuItems), TYPE_EXEC, 1, 1};

/* LpUart IT mode menu: wakeup on special character */
const tMenuItem LpUartIT2ModeMenuItems[] ={
{"", 14, 30, TYPE_EXEC, MODULE_NONE, LpUartWakeupIT2Demo, LpUartWakeupMenuUserAction, NULL, NULL },}
;
const tMenu LpUartIT2ModeMenu = {"WAKEUP: mode 2", LpUartIT2ModeMenuItems, countof(LpUartIT2ModeMenuItems), TYPE_EXEC, 1, 1};

/* Main menu LPUART wakeup*/
const tMenuItem LpUartWakeupMenuMenuItems[] =
{
    {TITLE_ANYCHAR,           14, 30,SEL_SUBMENU, MODULE_NONE, NULL, NULL, (const tMenu*)&LpUartIT1ModeMenu, NULL }, 
    {TITLE_SPECIALCHAR,       14, 30,SEL_SUBMENU, MODULE_NONE, NULL, NULL, (const tMenu*)&LpUartIT2ModeMenu, NULL }, 	
    {TITLE_RETURN,            0,  0,SEL_EXIT, MODULE_NONE, NULL, NULL, NULL, NULL }
};
const tMenu LpUartWakeupMenu = {TITLE_APPLI, LpUartWakeupMenuMenuItems, countof(LpUartWakeupMenuMenuItems), TYPE_TEXT, 1, 1 };

/* used to exit application */

__IO static uint8_t user_stop = 0;
__IO static uint8_t user_sel = 0;


/* Private typedef -----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void LpUartWakeupUserHeader(void);
static void Display_ExampleDescription(void);
static void Display_ExampleFirstPage(void);
static void Display_ExampleSecondPage(void);
static void Display_PageFooter(void);

static void Error_Handler(void);

static void StartIddMeasurement(void);
static void ManageIddMeasurementMenu(char * messTitle);

static void IddInfoDisplay(uint32_t iddvalueInNanoAmps,uint32_t iddState, uint32_t erroCode,char * messTitle);
static void InitUserInteractionInLPUART(void);

/* used to save GPIO context*/
void IDD_SaveContext(void);
void IDD_RestoreContext(void);
uint32_t GPIOA_MODER = 0, GPIOA_OTYPER = 0, GPIOA_OSPEEDR = 0, GPIOA_AFRL = 0, GPIOA_AFRH = 0; 
uint32_t GPIOB_MODER = 0, GPIOB_OTYPER = 0, GPIOB_OSPEEDR = 0, GPIOB_AFRL = 0, GPIOB_AFRH = 0; 
uint32_t GPIOC_MODER = 0, GPIOC_OTYPER = 0, GPIOC_OSPEEDR = 0, GPIOC_AFRL = 0, GPIOC_AFRH = 0; 
uint32_t GPIOD_MODER = 0, GPIOD_OTYPER = 0, GPIOD_OSPEEDR = 0, GPIOD_AFRL = 0, GPIOD_AFRH = 0;
uint32_t GPIOE_MODER = 0, GPIOE_OTYPER = 0, GPIOE_OSPEEDR = 0, GPIOE_AFRL = 0, GPIOE_AFRH = 0;
uint32_t GPIOH_MODER = 0, GPIOH_OTYPER = 0, GPIOH_OSPEEDR = 0, GPIOH_AFRL = 0, GPIOH_AFRH = 0;

/* External variables --------------------------------------------------------*/
const K_ModuleItem_Typedef LpUartWakepModuleItem =
{
  MODULE_LPUART_WAKEUP,
  _LpUartWakeupConfig,
  _LpUartWakeupDemoExec,
  _LpUartWakeupUnConfig,
  NULL
};  


/**
  * @brief  setup the HW for the LP Uart application
  * @param  None.
  * @note   set the memory + Hw initialisation.  
  * @retval None.
  */
KMODULE_RETURN _LpUartWakeupConfig(void)
{
  /* start the Joystick interrupt mode*/
  BSP_JOY_Init(JOY_MODE_GPIO);

		  
  /*##-Configure the UART peripheral ######################################*/
  /* Put the USART peripheral in the Asynchronous mode (UART Mode) */
  /* UART configured as follows:
      - Word Length = 8 Bits
      - Stop Bit    = One Stop bit
      - Parity      = ODD parity
      - BaudRate    = 9600 baud
      - Hardware flow control disabled (RTS and CTS signals) */
  UartHandle.Instance        = USARTx;

  UartHandle.Init.BaudRate   = 9600;
  UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
  UartHandle.Init.StopBits   = UART_STOPBITS_1;
  UartHandle.Init.Parity     = UART_PARITY_ODD;
  UartHandle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
  UartHandle.Init.Mode       = UART_MODE_TX_RX;
  
 if(HAL_UART_DeInit(&UartHandle) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UART_Init(&UartHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  return KMODULE_OK;
}

/**
  * @brief  un-Configure the LP Uart application 
  * @param  None.
  * @note   reset the memory + Hw initialisation.  
  * @retval None.
  */
KMODULE_RETURN _LpUartWakeupUnConfig(void)
{
  HAL_UART_DeInit(&UartHandle);

  return KMODULE_OK;
}

/**
  * @brief  Run the 8 uart application 
  * @param  None.
  * @note   run and display information about the uart transaction.  
  * @retval None.
  */
KMODULE_RETURN _LpUartWakeupDemoExec(void)
{
  /* Prepare the main MMI */
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  kMenu_Execute(LpUartWakeupMenu);
  
  /* Execute the app uart */
  /* App initialization    */
  return KMODULE_OK;
}

//=====================================================================
/**
  * @brief  System Power Configuration
  *         The system Power is configured as follow :
  *            + Regulator in LP mode
  *            + VREFINT OFF, with fast wakeup enabled
  *            + HSI as SysClk after Wake Up
  *            + No IWDG
  *            + Wakeup using EXTI Line (Key Button PC.13)
  * @param  None
  * @retval None
  */
static void SystemPower_Config(void)
{
  /* Enable Ultra low power mode */
  HAL_PWREx_EnableUltraLowPower();
  
  /* Enable the fast wake up from Ultra low power mode */
  HAL_PWREx_EnableFastWakeUp();
  
}

//=====================================================================
/**
  * @brief  Initialize the demo to handle wake through LP UART in IT mode
  * @param  Title displayed at the top of the display
  * @param  messActionToDo Message showing the action to do
  * @retval None
  */

static void InitUserInteractionInLPUART(void)
{
    
  /* Configure the system Power */
  SystemPower_Config();

  /* Force Re-init LPUART */
  HAL_UART_Init(&UartHandle);
  
  /* Save context */
  IDD_SaveContext();
    
  /* User push-button (EXTI_Line0) will be used to wakeup the system from STOP mode */
  /*__HAL_GPIO_EXTI_CLEAR_IT(TAMPER_BUTTON_PIN);
  BSP_PB_Init(BUTTON_TAMPER, BUTTON_MODE_EXTI); */
  
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED2);
  BSP_LED_Init(LED3);
  BSP_LED_Init(LED4);
  
  /* Force Re-init LPUART */
  HAL_UART_Init(&UartHandle);

  /*Enable UART Clock in Stop Mode*/
  HAL_UARTEx_EnableClockStopMode(&UartHandle);

  if (submenu_sel==1){
	  /* set the wake-up event:
	   * specify wake-up on start-bit detection */
	  WakeUpSelection.WakeUpEvent = UART_WAKEUP_ON_STARTBIT;
  }
  else{
	  /* set the wake-up event:
	  * specify address-to-match type. 
	  * The address is 0x2, meaning the character triggering the 
	  * address match is 0x82 */
	  WakeUpSelection.WakeUpEvent = UART_WAKEUP_ON_ADDRESS;
	  WakeUpSelection.AddressLength = UART_ADDRESS_DETECT_4B;
	  WakeUpSelection.Address = 0x2; 
  }
  
  if (HAL_UARTEx_StopModeWakeUpSourceConfig(&UartHandle, WakeUpSelection)!= HAL_OK)
  {
    Error_Handler(); 
  }

  /* Enable the UART Wake UP from stop mode Interrupt */
  __HAL_UART_ENABLE_IT(&UartHandle, UART_IT_WUF);
  
  /* enable MCU wake-up by UART */
  HAL_UARTEx_EnableStopMode(&UartHandle); 
  
}

//*******************************************************************************************
void LpUartWakeupIT1Demo(void)
{
  submenu_sel=1; 
  LpUartWakeupDemo();
}
//*******************************************************************************************
void LpUartWakeupIT2Demo(void)
{
  submenu_sel=2; 
  LpUartWakeupDemo();
}
//*******************************************************************************************
void LpUartWakeupDemo(void)
{
  user_stop = 0;

  while(user_stop == 0) 
  {
  
  /*Display user information Page1*/
  Display_ExampleFirstPage();

  /*switch off Led3 error*/
  BSP_LED_Off(LED3);
  /*switch on Led4*/
  BSP_LED_On(LED4);

  while(( user_sel == 0)&&(user_stop == 0));

  /*switch off Led4*/
  BSP_LED_Off(LED4);

  if(user_stop == 1)
      break;
  user_sel=0;

  /*Display user information Page2*/
  Display_ExampleSecondPage();
  
  /* Init the demo to handle wake through LPUART in DMA or IT mode*/
  InitUserInteractionInLPUART();
  
  /*switch on Led1*/   
   BSP_LED_On(LED1);
   
  /* Wakeup from any or special character entered on hyperterminal depending of mode selected*/  
  if (  submenu_sel==1 )
  {
    if (HAL_UART_Transmit_IT(&UartHandle, (uint8_t *)aTxBuffer1[0], (COUNTOF(aTxBuffer1[0]) - 1)) != HAL_OK)
    {
    /* Transfer error in transmission process */
    Error_Handler();
    }
  }
  else
  {
    if (HAL_UART_Transmit_IT(&UartHandle, (uint8_t *)aTxBuffer1[1], (COUNTOF(aTxBuffer1[1]) - 1)) != HAL_OK)
    {
      /* Transfer error in transmission process */
      Error_Handler();
    }
  }
  /* wait for 0.5 seconds before test start */
  HAL_Delay(500);
  
  /*##-Wake Up second step  ###############################################*/
  /* make sure that no UART transfer is on-going */ 
  while(__HAL_UART_GET_FLAG(&UartHandle, USART_ISR_BUSY) == SET);
  /* make sure that UART is ready to receive 
   * (test carried out again later in HAL_UARTEx_StopModeWakeUpSourceConfig) */    
  while(__HAL_UART_GET_FLAG(&UartHandle, USART_ISR_REACK) == RESET);
  
  /*start current measure*/
  StartIddMeasurement();

  /* Suspend the systTick in order to avoid being waked up */
  HAL_SuspendTick();

  /* enter stop mode */
  HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);

  /* Resume the systick */
  HAL_ResumeTick();

  /* at that point, MCU has been awoken: the LED has been turned back on */
  /* Wake Up on start bit detection successful */ 
  HAL_UARTEx_DisableStopMode(&UartHandle);
    
  /* wait for some delay */
  HAL_Delay(100);
  
  /* about to recover from stop mode: switch on LED2 */
  BSP_LED_On(LED2);
  

  /* Inform other board that wake up is successful just after wakeup in polling mode */
  if (HAL_UART_Transmit(&UartHandle, (uint8_t*)aWakeUpTrigger[submenu_sel-1], COUNTOF(aWakeUpTrigger[submenu_sel-1])-1, 1000)!= HAL_OK)  
  {
    Error_Handler();
  } 
  if (HAL_UART_Transmit(&UartHandle, (uint8_t*)aTxBuffer2[submenu_sel-1], COUNTOF(aTxBuffer2[submenu_sel-1])-1, 1000)!= HAL_OK)   
  {
    Error_Handler();
  } 

  /* Re-configure the system clock @ CPU max */
  SystemClock_Config();
  /* Restore context */
  IDD_RestoreContext();
  
  /* Measure the power */
  ManageIddMeasurementMenu(IDD_MEASURE_TITLE_STOP);
  }
}

/*******************************************************************************************/
/**
  * @brief  Display example Page1 message
  * @param  None
  * @retval None
  */
static void Display_ExampleFirstPage(void)
{
  /* Clear only LCD part*/
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_FillRect(0, 30, BSP_LCD_GetXSize(), BSP_LCD_GetYSize()-30);
 
  /*Display Joystick options*/
  Display_PageFooter();
  
  /* Display user information */
  LpUartWakeupUserHeader();
 
  /*Display LPUART config*/
  Display_ExampleDescription();
  
  
}
/**
  * @brief  Display example Page1 message
  * @param  None
  * @retval None
  */
static void Display_ExampleSecondPage(void)
{
  /* Clear only LCD part*/
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_FillRect(0, 30, BSP_LCD_GetXSize(), BSP_LCD_GetYSize()-30);
 
  
  /* Display user information */
  LpUartWakeupUserHeader();
 
  BSP_LCD_SetTextColor(LCD_COLOR_RED);  
  BSP_LCD_SetFont(&Font20);
  
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()/2-40, (uint8_t*)"Look on Hyperterminal,", CENTER_MODE); //LEFT_MODE
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()/2-20, (uint8_t*)"follow instructions,", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()/2, (uint8_t*)"Then come back on", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()/2+20, (uint8_t*)"the TFT display.", CENTER_MODE);
}
/**
  * @brief  Display Joystick options in Footer
  * @param  None
  * @retval None
  */
static void Display_PageFooter(void)
{
  /* Clear footer part*/
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
  BSP_LCD_FillRect(0, BSP_LCD_GetYSize()-30, BSP_LCD_GetXSize(), 30);
  
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetFont(&Font12);
  
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()-20 , (uint8_t*)"Press SEL to continue or Down to exit", CENTER_MODE);

}
/**
  * @brief  Display main example message
  * @param  None
  * @retval None
  */
static void Display_ExampleDescription(void)
{
  /*TFT screen 320*240*/
  uint8_t Ystart = 0;
  uint8_t Yend = 0;
  
  uint8_t id = 1;
  uint8_t step = 16;
   
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);  
  BSP_LCD_SetFont(&Font16);
  
  Ystart=BSP_LCD_GetYSize()/2 -105;  
  BSP_LCD_DisplayStringAt(0, Ystart+ id*step, (uint8_t*)"Please set JP10 to IDD,", CENTER_MODE); id++;
  BSP_LCD_DisplayStringAt(0, Ystart+ id*step, (uint8_t*)"JP12 to +3V3 and connect", CENTER_MODE); id++;
  BSP_LCD_DisplayStringAt(0, Ystart+ id*step, (uint8_t*)"an Hyperterminal with", CENTER_MODE); id++;
  BSP_LCD_DisplayStringAt(0, Ystart+ id*step, (uint8_t*)"following settings:", CENTER_MODE); id++;id++;
  
  Ystart=Ystart+id*step;
	
  /*Display LPUART config*/
  BSP_LCD_SetTextColor(LCD_COLOR_RED);  
  BSP_LCD_SetFont(&Font16);
  
  id=1;
  step=16;
  switch(submenu_sel)
    {   
    case 1:
	  BSP_LCD_DisplayStringAt(0, Ystart, (uint8_t*)"BaudRate = 9600", CENTER_MODE); 
      BSP_LCD_DisplayStringAt(0, Ystart+ id*step, (uint8_t*)"Data = 7 bits", CENTER_MODE); id++;
      BSP_LCD_DisplayStringAt(0, Ystart+ id*step, (uint8_t*)"Parity = ODD", CENTER_MODE); id++;
	  BSP_LCD_DisplayStringAt(0, Ystart+ id*step, (uint8_t*)"Stop = 1 bit", CENTER_MODE); id++;
	  BSP_LCD_DisplayStringAt(0, Ystart+ id*step, (uint8_t*)"HwFlowCtl = NONE", CENTER_MODE);
      break;
	  
    case 2:
      BSP_LCD_DisplayStringAt(0, Ystart, (uint8_t*)"BaudRate = 9600", CENTER_MODE); 
      BSP_LCD_DisplayStringAt(0, Ystart+ id*step, (uint8_t*)"Data = 7 bits", CENTER_MODE); id++;
      BSP_LCD_DisplayStringAt(0, Ystart+ id*step, (uint8_t*)"Parity = ODD", CENTER_MODE); id++;
	  BSP_LCD_DisplayStringAt(0, Ystart+ id*step, (uint8_t*)"Stop = 1 bit", CENTER_MODE); id++;
	  BSP_LCD_DisplayStringAt(0, Ystart+ id*step, (uint8_t*)"HwFlowCtl = NONE", CENTER_MODE);
	  break;
	  
	default:
      break;  
    }
	
  /*Draw rectangle*/
  Yend=Ystart+ id*step;
  HAL_Delay(500);
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_DrawRect(10, Yend-70, BSP_LCD_GetXSize()-20, 90);
  
}

/*=========================================================================*/
/**
  * @brief  This function is used to start the Idd measurement
  *         and it initialize the IID_BSP
  * @param  None
  * @retval None
  */
static void StartIddMeasurement()
{
  /* BSP_IDD_Reset(); Not to use since it resets joystick IT..*/
  /* Start the measurement */
  BSP_IDD_Init();
  BSP_IDD_ClearIT();
  BSP_IDD_DisableIT();
    
  BSP_IDD_ErrorClearIT();
  BSP_IDD_ErrorDisableIT();


  /* about to enter stop mode: switch off LED1 */
  BSP_LED_Off(LED1);

  /* Wait in order to be sure that the led is off */
  HAL_Delay(20);

  
  BSP_IDD_StartMeasure();
}

/**
  * @brief  Display the IDD power measurement
  * @param  Idd value in nanoAmps
  * @retval None
  */
static void IddInfoDisplay(uint32_t IddvalueInTenNanoAmps,
                           uint32_t IddState,
                           uint32_t ErrCode,
                           char * MessTitle)
{
  float TempIddDisplay = 0;

  /* Clear only LCD part */
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_FillRect(0, 20, BSP_LCD_GetXSize(), BSP_LCD_GetYSize()-50);
 
  /* Display Joystick options */
  Display_PageFooter();
  
  /* Display user information */
  LpUartWakeupUserHeader();
  
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);  
  BSP_LCD_SetFont(&Font20);
  
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()/2-40, (uint8_t *)MessTitle, CENTER_MODE);
  BSP_LCD_SetTextColor(LCD_COLOR_RED); 
  
  if (IddState == IDD_MEASURE_COMPLETED)
  {
    TempIddDisplay = (float) IddvalueInTenNanoAmps * 10;  /*Value in nano amps*/

    /* Value in uA */
    TempIddDisplay = TempIddDisplay/1000;
    if (TempIddDisplay < 1000)
    {
      sprintf((char*)LCDStr, " Idd = %-7.3f [uA] ", TempIddDisplay);
      BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()/2, (uint8_t *)LCDStr, CENTER_MODE);
  
    }
    else
    { /* Value in mA */
      TempIddDisplay = TempIddDisplay/1000;
      sprintf((char*)LCDStr, " Idd = %-7.3f [mA] ", TempIddDisplay);
      BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()/2, (uint8_t *)LCDStr, CENTER_MODE);
    }

    if (HAL_UART_Transmit(&UartHandle, (uint8_t*)aTxBuffer4, COUNTOF(aTxBuffer4)-1, 1000)!= HAL_OK)  
    {
      Error_Handler();
    }  
    if (HAL_UART_Transmit(&UartHandle, (uint8_t*)LCDStr, COUNTOF(LCDStr)-2, 1000)!= HAL_OK)
    {
      Error_Handler();
    }
    if (HAL_UART_Transmit(&UartHandle, (uint8_t*)aTxBuffer6, COUNTOF(aTxBuffer6)-1, 1000)!= HAL_OK)  
    {
      Error_Handler();
    }     
  }
  else
  {
    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()/2, (uint8_t *)"Idd measurement  ", CENTER_MODE);
    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()/2+20, (uint8_t *)"not available.   ", CENTER_MODE);

    if (HAL_UART_Transmit(&UartHandle, (uint8_t*)aTxBuffer4, COUNTOF(aTxBuffer4)-1, 1000)!= HAL_OK)  
    {
      Error_Handler();
    } 
    if (HAL_UART_Transmit(&UartHandle, (uint8_t*)aTxBuffer5, COUNTOF(aTxBuffer5)-1, 1000)!= HAL_OK)  
    {
      Error_Handler();
    }  
  }
  
  }
  

/**
  * @brief  This function is used to manage the display
  *         associated to the Idd measurement
  * @param  None
  * @retval None
  */
static void ManageIddMeasurementMenu(char * messTitle)
{
  uint32_t IddReadValue = 0;
  uint32_t JoyState = JOY_NONE_PIN;
  uint32_t IddState = IDD_STATE_UNCHANGED;
  uint32_t ErrorCode = 0;
  
  DecodeMFXInterrupt(&JoyState,&IddState);

  /* Current measure */
  if (IddState != IDD_ERR)
    IddState = IDD_MEASURE_COMPLETED;
	
  ErrorCode = (uint32_t)BSP_IDD_ErrorGetCode();
  BSP_IDD_GetValue(&IddReadValue);
  
  /* Display the results */
  IddInfoDisplay(IddReadValue,IddState,ErrorCode,messTitle);
  

  while(( user_sel == 0)&&( user_stop == 0));
  user_sel=0;

  /*switch off Led*/ 
  BSP_LED_Off(LED2);

}

/*=========================================================================*/

/**
  * @brief  Display uart transaction state 
  * @param  None.
  * @note   This display information about the uart transaction.  
  * @retval None.
  */
static void LpUartWakeupUserHeader(void)
{
   /* Set the Back Color */
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
  /* Set the Text Color */
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
}  

/**
  * @brief  Get User action 
  * @param  sel : User selection (JOY_SEL,...)
  * @note
  * @retval None
  */
void LpUartWakeupMenuUserAction(uint8_t sel)
{
  switch(sel)
  {
  case JOY_SEL : 
       user_sel = 1;
    break;
  case JOY_RIGHT : 
    break;
  case JOY_DOWN : 
       user_stop = 1;
    break;
  default :
    break;
  }
}


/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{

  /* Turn LED3 on */
  BSP_LED_On(LED3);
  while (1)
  {
  }
}

/**
  * @brief  Tx Transfer completed callback
  * @param  huart: UART handle.
  * @note   This example shows a simple way to report end of DMA Tx transfer, and
  *         you can add your own implementation.
  * @retval None
  */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{

  /* Turn LED1 on: Transfer in transmission process is correct */
  BSP_LED_On(LED1);

}

/**
  * @brief  Rx Transfer completed callback
  * @param  huart: UART handle
  * @note   This example shows a simple way to report end of DMA Rx transfer, and
  *         you can add your own implementation.
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

  /* Turn LED2 on: Transfer in reception process is correct */
  BSP_LED_On(LED2);

}

/**
  * @brief  UART error callbacks
  * @param  huart: UART handle
  * @note   This example shows a simple way to report transfer error, and you can
  *         add your own implementation.
  * @retval None
  */
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{

  /* Turn LED3 on: Transfer error in reception/transmission process */
  BSP_LED_On(LED3);

}

/*===================================================================*/

/**
* @brief Save Demonstration context (GPIOs Configurations, peripherals,...).
* @param None
* @retval None
*/
void IDD_SaveContext(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  /* Enable GPIOs clock */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  
  GPIOA_MODER = GPIOA->MODER;
  GPIOA_OTYPER = GPIOA->OTYPER;
  GPIOA_OSPEEDR = GPIOA->OSPEEDR;
  GPIOA_AFRL = GPIOA->AFR[0];
  GPIOA_AFRH = GPIOA->AFR[1];
  
  GPIOB_MODER = GPIOB->MODER;
  GPIOB_OTYPER = GPIOB->OTYPER;
  GPIOB_OSPEEDR = GPIOB->OSPEEDR;
  GPIOB_AFRL = GPIOB->AFR[0];
  GPIOB_AFRH = GPIOB->AFR[1];
  
  GPIOC_MODER = GPIOC->MODER;
  GPIOC_OTYPER = GPIOC->OTYPER;
  GPIOC_OSPEEDR = GPIOC->OSPEEDR;
  GPIOC_AFRL = GPIOC->AFR[0];
  GPIOC_AFRH = GPIOC->AFR[1];
  
  GPIOD_MODER = GPIOD->MODER;
  GPIOD_OTYPER = GPIOD->OTYPER;
  GPIOD_OSPEEDR = GPIOD->OSPEEDR;
  GPIOD_AFRL = GPIOD->AFR[0];
  GPIOD_AFRH = GPIOD->AFR[1];
  
  GPIOE_MODER = GPIOE->MODER;
  GPIOE_OTYPER = GPIOE->OTYPER;
  GPIOE_OSPEEDR = GPIOE->OSPEEDR;
  GPIOE_AFRL = GPIOE->AFR[0];
  GPIOE_AFRH = GPIOE->AFR[1];
  
  GPIOH_MODER = GPIOH->MODER;
  GPIOH_OTYPER = GPIOH->OTYPER;
  GPIOH_OSPEEDR = GPIOH->OSPEEDR;
  GPIOH_AFRL = GPIOH->AFR[0];
  GPIOH_AFRH = GPIOH->AFR[1];
  
  /* Configure all GPIO port pins in Analog Input mode (floating input trigger OFF) */
  GPIO_InitStructure.Pin = GPIO_PIN_All;
  GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStructure.Pull = GPIO_NOPULL;

  HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);   /*DEBUG*/
  HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);   /*I2C, MFX*/
  HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);   /*TAMPER button*/
  HAL_GPIO_Init(GPIOE, &GPIO_InitStructure);   /*TFT display*/
  
  /*
  GPIO_InitStructure.Pin = (GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | \
                            GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | \
                            GPIO_PIN_14 | GPIO_PIN_15); 
  HAL_GPIO_Init(GPIOD, &GPIO_InitStructure);       
  */
  
  GPIO_InitStructure.Pin = (GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_9 | GPIO_PIN_10);
  GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStructure.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOH, &GPIO_InitStructure);
  
  /* Disable GPIOs clock */
  __HAL_RCC_GPIOA_CLK_DISABLE();
  __HAL_RCC_GPIOB_CLK_DISABLE();
  __HAL_RCC_GPIOC_CLK_DISABLE();
  /*__HAL_RCC_GPIOD_CLK_DISABLE();*/
  __HAL_RCC_GPIOE_CLK_DISABLE();
  __HAL_RCC_GPIOH_CLK_DISABLE();

  /* SPI Clock disable */
  /*__HAL_RCC_SPI1_CLK_DISABLE();*/
}

/**
* @brief Restore Demonstration context (GPIOs Configurations, peripherals,...).
* @param None
* @retval None
*/
void IDD_RestoreContext(void)
{
  /* Enable GPIOs clock */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  

  GPIOA->MODER = GPIOA_MODER;
  GPIOA->OTYPER = GPIOA_OTYPER ;
  GPIOA->OSPEEDR = GPIOA_OSPEEDR;
  GPIOA->AFR[0] = GPIOA_AFRL;
  GPIOA->AFR[1] = GPIOA_AFRH;

  GPIOB->MODER = GPIOB_MODER;
  GPIOB->OTYPER = GPIOB_OTYPER ;
  GPIOB->OSPEEDR = GPIOB_OSPEEDR;
  GPIOB->AFR[0] = GPIOB_AFRL;
  GPIOB->AFR[1] = GPIOB_AFRH;

  GPIOC->MODER = GPIOC_MODER;
  GPIOC->OTYPER = GPIOC_OTYPER ;
  GPIOC->OSPEEDR = GPIOC_OSPEEDR;
  GPIOC->AFR[0] = GPIOC_AFRL;
  GPIOC->AFR[1] = GPIOC_AFRH;

  GPIOD->MODER = GPIOD_MODER;
  GPIOD->OTYPER = GPIOD_OTYPER ;
  GPIOD->OSPEEDR = GPIOD_OSPEEDR;
  GPIOD->AFR[0] = GPIOD_AFRL;
  GPIOD->AFR[1] = GPIOD_AFRH;

  GPIOE->MODER = GPIOE_MODER;
  GPIOE->OTYPER = GPIOE_OTYPER ;
  GPIOE->OSPEEDR = GPIOE_OSPEEDR;
  GPIOE->AFR[0] = GPIOE_AFRL;
  GPIOE->AFR[1] = GPIOE_AFRH;
  
  GPIOH->MODER = GPIOH_MODER;
  GPIOH->OTYPER = GPIOH_OTYPER ;
  GPIOH->OSPEEDR = GPIOH_OSPEEDR;
  GPIOH->AFR[0] = GPIOH_AFRL;
  GPIOH->AFR[1] = GPIOH_AFRH;
  
  /* Initialize the User button */
  /*BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_EXTI);*/

  /* Enable the SPI Clock */
  /*__HAL_RCC_SPI1_CLK_ENABLE();*/

}


/*===================================================================*/
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

