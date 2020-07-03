/**
  ******************************************************************************
  * @file    menu.c 
  * @author  MCD Application Team
  * @brief   Main program body
  *          This example code shows how to use the Nucleo BSP Drivers
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
#include "menu.h"
#include "bsp.h"
#include "main.h"
#include "../Modules/iddmeasurement/power.h"
#include "../Modules/usbHID/usbdapp.h"
#include "../Core/Src/menu_res.c"
#include "../Modules/usbHID/usbdevice_res.c"
#include "../Modules/iddmeasurement/idd_res.c"

/** @addtogroup CORE
  * @{
  */

/** @defgroup BSP
  * @brief BSP routines
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern uint8_t str[20];
/* TSC handler declaration */
extern TSC_HandleTypeDef TscHandle;
/* USB handler declaration */
extern USBD_HandleTypeDef USBD_Device;

__IO MENU_Select_State  menu_state;
__IO uint32_t ButtonPressed = 0x00;

uint32_t GPIOA_MODER = 0, GPIOA_OTYPER = 0, GPIOA_OSPEEDR = 0, GPIOA_AFRL = 0, GPIOA_AFRH = 0; 
uint32_t GPIOB_MODER = 0, GPIOB_OTYPER = 0, GPIOB_OSPEEDR = 0, GPIOB_AFRL = 0, GPIOB_AFRH = 0; 
uint32_t GPIOC_MODER = 0, GPIOC_OTYPER = 0, GPIOC_OSPEEDR = 0, GPIOC_AFRL = 0, GPIOC_AFRH = 0; 
uint32_t GPIOD_MODER = 0, GPIOD_OTYPER = 0, GPIOD_OSPEEDR = 0, GPIOD_AFRL = 0, GPIOD_AFRH = 0;
uint32_t GPIOH_MODER = 0, GPIOH_OTYPER = 0, GPIOH_OSPEEDR = 0, GPIOH_AFRL = 0, GPIOH_AFRH = 0;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/**
  * @brief  Manages Menu Process.
  * @param  None
  * @retval None
  */
void Menu_Process(void)
{
  tsl_user_status_t tsl_status;
  
  /* background menu: entry menu */
  switch(menu_state)
  {
  case BACKGROUND_MENU:
    BSP_EPD_SetFont(&Font16);
    BSP_EPD_DrawImage(0, 0, 72, 172, Background_pic);
    BSP_EPD_DisplayStringAt(0, 13, (uint8_t*)"STM32L053", RIGHT_MODE);
    BSP_EPD_SetFont(&Font12);
    BSP_EPD_DisplayStringAt(87, 8, (uint8_t*)"DISCOVERY", LEFT_MODE);
    BSP_EPD_DisplayStringAt(71, 3, (uint8_t*)"DEMONSTRATION", LEFT_MODE);
    BSP_EPD_DisplayStringAt(94, 0, (uint8_t*)"V1.0.0", LEFT_MODE);
    BSP_EPD_RefreshDisplay();
    HAL_Delay(2000);
    menu_state = MAIN_MENU;
    break;

  /* main menu */
  case MAIN_MENU:
    BSP_EPD_Clear(EPD_COLOR_WHITE);
    BSP_EPD_DrawImage(5, 0, 48, 48, Mouse_pic);
    BSP_EPD_DrawImage(130, 0, 48, 26, Batery_pic);
    BSP_EPD_DrawImage(60, 0, 16, 60, Slider_pic);
    BSP_EPD_SetFont(&Font12);
    BSP_EPD_DisplayStringAt(0, 13, (uint8_t*)"STM32L053 Demonstration", CENTER_MODE);
    BSP_EPD_DisplayStringAt(56, 7, (uint8_t*)"USE LTS to", LEFT_MODE);
    BSP_EPD_DisplayStringAt(53, 4, (uint8_t*)"Select Demo", LEFT_MODE);
    BSP_EPD_DrawRect(2, 12, 166, 5);
    BSP_EPD_RefreshDisplay();
    menu_state = MODULE_MENU;
    break;
    
  case MODULE_MENU:
    MyLinRots[0].p_Data->Position = 0;
    /* Execute STMTouch Driver state machine */
    tsl_status = tsl_user_Exec();
    if(tsl_status != TSL_USER_STATUS_BUSY)
    {
      if(LINEAR_DETECT)
      {
        /* USB Module Menu*****************************************************/
        if((LINEAR_POSITION >= 1 ) && (LINEAR_POSITION <= 127))
        {
          menu_state = USB_MENU;
        }  
        /* PWR Module Menu*****************************************************/
        else if ((LINEAR_POSITION >= 128 ) && (LINEAR_POSITION < 255))
        {
          menu_state = PWR_MENU;
        }
      }
    }
    break;

  case USB_MENU:
    BSP_EPD_Clear(EPD_COLOR_WHITE);
    BSP_EPD_DrawImage(13, 4, 28, 146, FullSlider_pic);
    BSP_EPD_SetFont(&Font12);
    BSP_EPD_DisplayStringAt(0, 13, (uint8_t*)"STM32L053 USB Demo", CENTER_MODE);
    BSP_EPD_DisplayStringAt(15, 1, (uint8_t*)"LEFT RIGHT UP DOWN", LEFT_MODE);
    BSP_EPD_DrawRect(2, 12, 166, 5);
    BSP_EPD_DrawRect(145, 0, 26, 4);
    BSP_EPD_SetFont(&Font8);
    BSP_EPD_DisplayStringAt(146, 1, (uint8_t*)"B1 Ex", LEFT_MODE);
    BSP_EPD_RefreshDisplay();
    MyLinRots[0].p_Data->Position = 0;

    /* USB Clock config */
    USBClock_Enable();
    /* USB Initialization */
    BSP_USB_Init();
    
    ButtonPressed = 0;
    while(ButtonPressed == 0)
    {
      /* Execute STMTouch Driver state machine */
      tsl_status = tsl_user_Exec();
      if(tsl_status != TSL_USER_STATUS_BUSY)
      {
        USB_process(tsl_status);
      }
    }
    /* USB Clock disable */
    USBClock_Disable();
    /* USB DeInitialize */
    USBD_DeInit(&USBD_Device);

    ButtonPressed = 0;
    menu_state = MAIN_MENU;
    break;

  case PWR_MENU:
    BSP_LED_Off(LED4);
    BSP_LED_Off(LED3);
    /* Power Main IDD Menu ****************************************************/
    BSP_EPD_Clear(EPD_COLOR_WHITE);
    BSP_EPD_DrawImage(30, 0, 16, 60, Slider_pic);
    BSP_EPD_DrawImage(3, 4, 32, 32, Run_pic);
    BSP_EPD_DrawImage(50, 4, 32, 29, Sleep_pic);
    BSP_EPD_DrawImage(90, 4, 32, 29, LPSleep_pic);
    BSP_EPD_DrawImage(130, 4, 32, 32, Stop_pic);
    BSP_EPD_SetFont(&Font12);
    BSP_EPD_DisplayStringAt(0, 13, (uint8_t*)"STM32L053 IDD Demo", CENTER_MODE);
    BSP_EPD_DrawRect(2, 12, 166, 5);
    BSP_EPD_DrawRect(113, 0, 57, 3);
    BSP_EPD_SetFont(&Font8);
    BSP_EPD_DisplayStringAt(116, 1, (uint8_t*)"B1 to Exit", LEFT_MODE);
    BSP_EPD_RefreshDisplay();
   
    MyLinRots[0].p_Data->Position = 0;
    MyLinRots[0].p_Data->StateId = TSL_STATEID_RELEASE;
    ButtonPressed = 0;
    tsl_status = tsl_user_Exec();
    while((ButtonPressed == 0) && ((MyLinRots[0].p_Data->StateId != TSL_STATEID_DETECT) || (LINEAR_POSITION == 0)) && (!LINEAR_POSITION))
    {
      tsl_status = tsl_user_Exec();
    }
    
    if(MyLinRots[0].p_Data->StateId  == TSL_STATEID_DETECT)
    {
      /* Run mode menu ********************************************************/
      if((LINEAR_POSITION >= 1 ) && (LINEAR_POSITION <= 64))
      {
        Idd_Run_process();
        BSP_EPD_Clear(EPD_COLOR_WHITE);
        BSP_EPD_SetFont(&Font12);
        BSP_EPD_DisplayStringAt(0, 13, (uint8_t*)"MCU in Run Mode", CENTER_MODE);
        BSP_EPD_DrawRect(2, 12, 166, 5);
        BSP_EPD_DisplayStringAt(8, 7, (uint8_t*)"Current consumption", LEFT_MODE);
        BSP_EPD_DisplayStringAt(8, 3, str, LEFT_MODE);
        BSP_EPD_DrawRect(113, 0, 57, 4);
        BSP_EPD_SetFont(&Font8);
        BSP_EPD_DisplayStringAt(116, 1, (uint8_t*)"B1 to Exit", LEFT_MODE);
        BSP_EPD_RefreshDisplay();
      }
      
      /* Sleep mode menu ******************************************************/
      else if ((LINEAR_POSITION >= 65 ) && (LINEAR_POSITION <= 127))
      {
        /* Display before entering Sleep mode */
        BSP_EPD_Clear(EPD_COLOR_WHITE);
        BSP_EPD_SetFont(&Font12);
        BSP_EPD_DisplayStringAt(0, 13, (uint8_t*)"MCU in Sleep Mode", CENTER_MODE);
        BSP_EPD_DrawRect(2, 12, 166, 5);
        BSP_EPD_RefreshDisplay();
        
        /* Execute Sleep */
        Idd_Sleep_process();
        
        /* Display after wakeup from Sleep mode */
        BSP_EPD_Clear(EPD_COLOR_WHITE);
        BSP_EPD_DisplayStringAt(0, 13, (uint8_t*)"Wake up from Sleep", CENTER_MODE);
        BSP_EPD_DrawRect(2, 12, 166, 5);
        BSP_EPD_DisplayStringAt(8, 7, (uint8_t*)"Current consumption", LEFT_MODE);
        BSP_EPD_DisplayStringAt(8, 3, str, LEFT_MODE);
        BSP_EPD_DrawRect(113, 0, 57, 4);
        BSP_EPD_SetFont(&Font8);
        BSP_EPD_DisplayStringAt(116, 1, (uint8_t*)"B1 to Exit", LEFT_MODE);
        BSP_EPD_RefreshDisplay();
      }
      
      /* LP Sleep menu ********************************************************/
      else if ((LINEAR_POSITION >= 128 ) && (LINEAR_POSITION <= 191))
      {
        /* Display before entering LP Sleep mode */
        BSP_EPD_Clear(EPD_COLOR_WHITE);
        BSP_EPD_SetFont(&Font12);
        BSP_EPD_DisplayStringAt(0, 13, (uint8_t*)"MCU in LPSleep Mode", CENTER_MODE);
        BSP_EPD_DrawRect(2, 12, 166, 5);
        BSP_EPD_RefreshDisplay();
         
        /* Execute LP SLEEP */
        Idd_LPSleep_process();
        
        /* Display after wakeup from LP Sleep mode */
        BSP_EPD_Clear(EPD_COLOR_WHITE);
        BSP_EPD_DisplayStringAt(0, 13, (uint8_t*)"Wake up from LPSleep", CENTER_MODE);
        BSP_EPD_DrawRect(2, 12, 166, 5);
        BSP_EPD_DisplayStringAt(8, 7, (uint8_t*)"Current consumption", LEFT_MODE);
        BSP_EPD_DisplayStringAt(8, 3, str, LEFT_MODE);
        BSP_EPD_DrawRect(113, 0, 57, 4);
        BSP_EPD_SetFont(&Font8);
        BSP_EPD_DisplayStringAt(116, 1, (uint8_t*)"B1 to Exit", LEFT_MODE);
        BSP_EPD_RefreshDisplay();
      }
      /* Stop menu ************************************************************/
      else
      {
        /* Display before entering Stop mode */
        BSP_EPD_Clear(EPD_COLOR_WHITE);
        BSP_EPD_SetFont(&Font12);
        BSP_EPD_DisplayStringAt(0, 13, (uint8_t*)"MCU in Stop Mode", CENTER_MODE);
        BSP_EPD_DrawRect(2, 12, 166, 5);
        BSP_EPD_RefreshDisplay();
        
        /* Execute STOP Process */
        Idd_Stop_process();
        
        /* Display after wakeup from Stop mode */
        BSP_EPD_Clear(EPD_COLOR_WHITE);
        BSP_EPD_DisplayStringAt(0, 13, (uint8_t*)"Wake up from STOP", CENTER_MODE);
        BSP_EPD_DrawRect(2, 12, 166, 5);
        BSP_EPD_DisplayStringAt(8, 7, (uint8_t*)"Current consumption", LEFT_MODE);
        BSP_EPD_DisplayStringAt(8, 3, str, LEFT_MODE);
        BSP_EPD_DrawRect(113, 0, 57, 4);
        BSP_EPD_SetFont(&Font8);
        BSP_EPD_DisplayStringAt(116, 1, (uint8_t*)"B1 to Exit", LEFT_MODE);
        BSP_EPD_RefreshDisplay();
      }
      
      ButtonPressed = 0;
      while(ButtonPressed == 0)
      {
      }
      menu_state = PWR_MENU;
    }
    else /* BUTTON_KEY != 0 */
    {
      menu_state = MAIN_MENU;
    }

    break;
    
  default:
    break;
  }
}

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
  
  GPIOH_MODER = GPIOH->MODER;
  GPIOH_OTYPER = GPIOH->OTYPER;
  GPIOH_OSPEEDR = GPIOH->OSPEEDR;
  GPIOH_AFRL = GPIOH->AFR[0];
  GPIOH_AFRH = GPIOH->AFR[1];
  
  /* Configure all GPIO port pins in Analog Input mode (floating input trigger OFF) */
  GPIO_InitStructure.Pin = GPIO_PIN_All;
  GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStructure.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
  HAL_GPIO_Init(GPIOB, &GPIO_InitStructure); 
  HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);
  HAL_GPIO_Init(GPIOD, &GPIO_InitStructure); 
  HAL_GPIO_Init(GPIOH, &GPIO_InitStructure);
  
  /* Disable GPIOs clock */
  __HAL_RCC_GPIOA_CLK_DISABLE();
  __HAL_RCC_GPIOB_CLK_DISABLE();
  __HAL_RCC_GPIOC_CLK_DISABLE();
  __HAL_RCC_GPIOD_CLK_DISABLE();
  __HAL_RCC_GPIOH_CLK_DISABLE();
  
  HAL_TSC_DeInit(&TscHandle);
 
  /* TSC Clock disable */
  __HAL_RCC_TSC_CLK_DISABLE();
  __HAL_RCC_TSC_FORCE_RESET();
  __HAL_RCC_TSC_RELEASE_RESET();
  
  /* SPI Clock disable */
  __HAL_RCC_SPI1_CLK_DISABLE();
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
  
  GPIOH->MODER = GPIOH_MODER;
  GPIOH->OTYPER = GPIOH_OTYPER ;
  GPIOH->OSPEEDR = GPIOH_OSPEEDR;
  GPIOH->AFR[0] = GPIOH_AFRL;
  GPIOH->AFR[1] = GPIOH_AFRH;
  
  
  /* Initialize the User button */
  BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_EXTI);
  __HAL_RCC_TSC_CLK_ENABLE();
  
  /* Initialize the Touch sensor */
  BSP_TSL_Init();
  
  /* Enable the SPI Clock */
  __HAL_RCC_SPI1_CLK_ENABLE();

  /* Initialize the EPD */
  BSP_EPD_Init();
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = HSI
  *            SYSCLK(Hz)                     = 16000000
  *            HCLK(Hz)                       = 16000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            APB2 Prescaler                 = 1
  *            HSI Frequency(Hz)              = 16000000
  *            Flash Latency(WS)              = 1
  *            Main regulator output voltage  = Scale1 mode
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
 
   /* Enable Power Controller clock */
  __HAL_RCC_PWR_CLK_ENABLE();
  
  /* The voltage scaling allows optimizing the power consumption when the device is 
  clocked below the maximum system frequency, to update the voltage scaling value 
  regarding system frequency refer to product datasheet. */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2); 
  
  /* Enable HSI Oscillator to be used as System clock source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);
  
  /* Select HSI as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clock dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1);

  /* Select HSI as system clock source after Wake Up from Stop mode */
  __HAL_RCC_WAKEUPSTOP_CLK_CONFIG(RCC_STOP_WAKEUPCLOCK_HSI);

}

/**
  * @brief  USB Clock Configuration: Enable Resources
  * @param  None
  * @retval None
  */
void USBClock_Enable(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;
  static RCC_CRSInitTypeDef RCC_CRSInitStruct;

  /* Enable HSI48 Oscillator to be used as USB clock source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48;
  RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);
  /* Select HSI48 as USB clock source */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USB;
  PeriphClkInitStruct.UsbClockSelection = RCC_USBCLKSOURCE_HSI48;
  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
  
  /*Configure the clock recovery system (CRS)**********************************/
  
  /*Enable CRS Clock*/
  __HAL_RCC_CRS_CLK_ENABLE(); 
  
  /* Default Synchro Signal division factor (not divided) */
  RCC_CRSInitStruct.Prescaler = RCC_CRS_SYNC_DIV1;  
  /* Set the SYNCSRC[1:0] bits according to CRS_Source value */
  RCC_CRSInitStruct.Source = RCC_CRS_SYNC_SOURCE_USB;  
  /* HSI48 is synchronized with USB SOF at 1KHz rate */
  RCC_CRSInitStruct.ReloadValue =  __HAL_RCC_CRS_RELOADVALUE_CALCULATE(48000000, 1000);
  RCC_CRSInitStruct.ErrorLimitValue = RCC_CRS_ERRORLIMIT_DEFAULT;  
  /* Set the TRIM[5:0] to the default value*/
  RCC_CRSInitStruct.HSI48CalibrationValue = 0x20;   
  /* Start automatic synchronization */ 
  HAL_RCCEx_CRSConfig (&RCC_CRSInitStruct);
}


/**
  * @brief  USB Clock Configuration: Disable Resources
  * @param  None
  * @retval None
  */
void USBClock_Disable(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct;

  /* Enable HSI48 Oscillator to be used as USB clock source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48;
  RCC_OscInitStruct.HSI48State = RCC_HSI48_OFF;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  /*Enable CRS Clock*/
  __HAL_RCC_CRS_CLK_DISABLE();
}

/**
  * @brief EXTI line detection callback.
  * @param GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if(GPIO_Pin == GPIO_PIN_0)
  {
    ButtonPressed = 1;
  }
}

/**
  * @}
  */

/**
  * @}
  *
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
