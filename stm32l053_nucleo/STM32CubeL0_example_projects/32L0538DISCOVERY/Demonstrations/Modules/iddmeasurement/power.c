/**
  ******************************************************************************
  * @file    power.c 
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
#include "main.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* I2C handler declaration */
I2C_HandleTypeDef I2CxHandle;

uint8_t str[20];
uint8_t param[20];
uint32_t Idd_measurement;

/* Buffer used for reception */
uint8_t aRxBuffer[3];
__IO uint32_t RunModeMeasure = 0x00;

/* Private function prototypes -----------------------------------------------*/
static void I2C_Config(void);
static void Error_Handler(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Process for measuring the IDD in Run mode
  * @param  None
  * @retval None
  */
void Idd_Run_process(void)
{
  /* Configure the I2C */
  I2C_Config();

  /* ######################### MFX Configuration ############################ */
  param[0]=0x80;
  if(HAL_I2C_Mem_Write(&I2CxHandle, 0x84, 0x40, I2C_MEMADD_SIZE_8BIT, param, 0x1, 10000) != HAL_OK)
  {
    Error_Handler();
  }

  /* Required delay to reset the MFX */
  HAL_Delay(100);

  /* IRQ pin in push-pull, active high */
  param[0]=0x03;
  if(HAL_I2C_Mem_Write(&I2CxHandle, 0x84, 0x41, I2C_MEMADD_SIZE_8BIT, param, 0x1, 10000) != HAL_OK)
  {
    Error_Handler();
  }
  /* Delay required for MFX to change IRQ_out pin config, before activate it on STM32L0 SW */
  HAL_Delay(1);
  
  /* IRQ_SRC: ERROR and IDD */
  param[0]=0x06;  
  if(HAL_I2C_Mem_Write(&I2CxHandle, 0x84, 0x42, I2C_MEMADD_SIZE_8BIT, param, 0x1, 10000) != HAL_OK)
  {
    Error_Handler();
  }
  
  /* Enable IDD function */
  param[0]=0x04;
  if(HAL_I2C_Mem_Write(&I2CxHandle, 0x84, 0x40, I2C_MEMADD_SIZE_8BIT, param, 0x1, 10000) != HAL_OK)
  {
    Error_Handler();
  }
  
  /* SH0 1000 mohm */
  param[0]=0x03; param[1]=0xE8;
  /* SH1 24 ohm */
  param[2]=0x00; param[3]=0x18;
  /* SH2 620 ohm */
  param[4]=0x02; param[5]=0x6C; 
  /* SH3 not exist with STM32L0538 discovery */
  param[6]=0x00; param[7]=0x00;
  /* SH4 10000 ohm */
  param[8]=0x27; param[9]=0x10;
  /* gain x49.9 (4990)*/
  param[10]=0x13; param[11]=0x7E;
  /* VDD_MCU min 3000mV (0xBB8) for undershoot detection */
  param[12]=0x0B; param[13]=0xB8;
  /* Send 12 parameters */
  if(HAL_I2C_Mem_Write(&I2CxHandle, 0x84, 0x82, I2C_MEMADD_SIZE_8BIT, param, 0xE, 10000) != HAL_OK)  
  {
    Error_Handler();
  }
  
  RunModeMeasure = 0x01;
  BSP_LED_Off(LED3);
  BSP_LED_Off(LED4);

  /* Request IDD measurement with 4 shunts */
  param[0]=0x9;
  if(HAL_I2C_Mem_Write(&I2CxHandle, 0x84, 0x80, I2C_MEMADD_SIZE_8BIT, param, 0x1, 10000) != HAL_OK)
  {
    Error_Handler();
  }
  /* Waiting 100ms delay before sending read request to MFX */
  HAL_Delay(100);
  
  aRxBuffer[0] = 0x00;
  aRxBuffer[1] = 0x00;
  aRxBuffer[2] = 0x00;
  RunModeMeasure = 0x00;

  /* Idd measurement */
  if(HAL_I2C_Mem_Read(&I2CxHandle, 0x84, 0x14, I2C_MEMADD_SIZE_8BIT, (uint8_t*)aRxBuffer, 3, 10000) != HAL_OK)
  {
    Error_Handler();
  } 
  
  /* Acknowledge IDD IRQ from MFX */
  param[0]=0x2;
  if(HAL_I2C_Mem_Write(&I2CxHandle, 0x84, 0x44, I2C_MEMADD_SIZE_8BIT, param, 0x1, 10000) != HAL_OK)
  {
    Error_Handler();
  }
  /* delay for MFX to remove IRQ signal */
  HAL_Delay(1);
  
  /* Get the IDD measurement value */
  Idd_measurement = (aRxBuffer[0]<<16) + (aRxBuffer[1]<<8) + aRxBuffer[2] ;
  Idd_measurement = Idd_measurement * 10;

  sprintf((char*)str, "%d,%.3dmA", (int)Idd_measurement/1000000, ((int)Idd_measurement%1000000)/1000);
}

/**
  * @brief  Process for measuring the IDD in Sleep mode
  * @param  None
  * @retval None
  */
void Idd_Sleep_process(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  
  /* Configure the I2C */
  I2C_Config();
  
  /* ######################### MFX Configuration ############################ */
  param[0]=0x80;
  if(HAL_I2C_Mem_Write(&I2CxHandle, 0x84, 0x40, I2C_MEMADD_SIZE_8BIT, param, 0x1, 10000) != HAL_OK)
  {
    Error_Handler();
  }
  
  /* Required delay to reset MFX */  
  HAL_Delay(100);

  /* IRQ pin in push-pull, active high */
  param[0]=0x03;
  if(HAL_I2C_Mem_Write(&I2CxHandle, 0x84, 0x41, I2C_MEMADD_SIZE_8BIT, param, 0x1, 10000) != HAL_OK)
  {
    Error_Handler();
  }

  /* Delay required for MFX to change IRQ_out pin config, before activate it on STM32L0 SW */
  HAL_Delay(1);
  
  /* IRQ_SRC: ERROR and IDD */
  param[0]=0x06;  
  if(HAL_I2C_Mem_Write(&I2CxHandle, 0x84, 0x42, I2C_MEMADD_SIZE_8BIT, param, 0x1, 10000) != HAL_OK)
  {
    Error_Handler();
  }
  
  /* Enable IDD function */
  param[0]=0x04;
  if(HAL_I2C_Mem_Write(&I2CxHandle, 0x84, 0x40, I2C_MEMADD_SIZE_8BIT, param, 0x1, 10000) != HAL_OK)
  {
    Error_Handler();
  }
  
  /* SH0 1000 mohm */
  param[0]=0x03; param[1]=0xE8; 
  /* SH1 24 ohm */
  param[2]=0x00; param[3]=0x18;
  /* SH2 620 ohm */
  param[4]=0x02; param[5]=0x6C; 
  /* SH3 not exist with STM32L0538 discovery */
  param[6]=0x00; param[7]=0x00; 
  /* SH4 10000 ohm */
  param[8]=0x27; param[9]=0x10; 
  /* gain x49.9 (4990) */
  param[10]=0x13; param[11]=0x7E; 
  /* VDD_MCU min 3000mV (0xBB8) for undershoot detection */
  param[12]=0x0B; param[13]=0xB8; 
  /* send 12 parameters */
  if(HAL_I2C_Mem_Write(&I2CxHandle, 0x84, 0x82, I2C_MEMADD_SIZE_8BIT, param, 0xE, 10000) != HAL_OK)  
  {
    Error_Handler();
  }
  
  /* Request IDD measurement with 4 shunts */
  param[0]=0x9;
  if(HAL_I2C_Mem_Write(&I2CxHandle, 0x84, 0x80, I2C_MEMADD_SIZE_8BIT, param, 0x1, 10000) != HAL_OK)
  {
    Error_Handler();
  }
  
  /* Add a predelay before Idd measurement */
  param[0]=0xFF;
  if(HAL_I2C_Mem_Write(&I2CxHandle, 0x84, 0x81, I2C_MEMADD_SIZE_8BIT, param, 0x1, 10000) != HAL_OK)
  {
    Error_Handler();
  }
  
  /* Save context */
  IDD_SaveContext();
  
  /* Deinitialize the I2C */
  HAL_I2C_DeInit(&I2CxHandle);
   
   __HAL_RCC_GPIOC_CLK_ENABLE();
   /* Configure Button pin as input with External interrupt: Enable MFX IT generation on this SW */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH  ;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
  
  /* Enable and set Button EXTI Interrupt to the lowest priority */
  HAL_NVIC_SetPriority((IRQn_Type)(EXTI4_15_IRQn), 3, 0);
  HAL_NVIC_EnableIRQ((IRQn_Type)(EXTI4_15_IRQn));
  
  /*Suspend Tick increment to prevent wakeup by Systick interrupt. 
  Otherwise the Systick interrupt will wake up the device within 1ms (HAL time base)*/
  HAL_SuspendTick();
  
  /* Enter Sleep Mode , wake up is done once Key push button is pressed */
  HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
  
  /* Resume Tick interrupt if disabled prior to sleep mode entry*/
  HAL_ResumeTick();
  
  /* Restore context */
  IDD_RestoreContext();
  
  /* Configure the I2C */
  I2C_Config();
  
  aRxBuffer[0] = 0x00;
  aRxBuffer[1] = 0x00;
  aRxBuffer[2] = 0x00;
  
  /* Idd measurement */
  if(HAL_I2C_Mem_Read(&I2CxHandle, 0x84, 0x14, I2C_MEMADD_SIZE_8BIT, (uint8_t*)aRxBuffer, 3, 10000) != HAL_OK)
  {
    Error_Handler();
  } 
  
  /* Acknowledge IDD IRQ from MFX */
  param[0]=0x2;
  if(HAL_I2C_Mem_Write(&I2CxHandle, 0x84, 0x44, I2C_MEMADD_SIZE_8BIT, param, 0x1, 10000) != HAL_OK)
  {
    Error_Handler();
  }
  /* delay for MFX to remove IRQ signal */
  HAL_Delay(1);  
  
  /* Get the IDD measurement value */
  Idd_measurement = (aRxBuffer[0]<<16) + (aRxBuffer[1]<<8) + aRxBuffer[2] ;
  Idd_measurement = Idd_measurement * 10;
  sprintf((char*)str, "%d,%.3dmA", (int)Idd_measurement/1000000, ((int)Idd_measurement%1000000)/1000);
}

/**
  * @brief  Process for measuring the IDD in LP Sleep mode
  * @param  None
  * @retval None
  */
void Idd_LPSleep_process(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
 
  /* Save context */
  IDD_SaveContext(); 
  
  /* Disable Prefetch Buffer */
  __HAL_FLASH_PREFETCH_BUFFER_DISABLE();
  
  /* Configure the I2C */
  I2C_Config();

  /* ######################### MFX Configuration ############################ */
  param[0]=0x80;
  if(HAL_I2C_Mem_Write(&I2CxHandle, 0x84, 0x40, I2C_MEMADD_SIZE_8BIT, param, 0x1, 10000) != HAL_OK)
  {
    Error_Handler();
  }
  /* Required delay to reset MFX */  
  HAL_Delay(100);

  /* IRQ pin in push-pull, active high */
  param[0]=0x03;
  if(HAL_I2C_Mem_Write(&I2CxHandle, 0x84, 0x41, I2C_MEMADD_SIZE_8BIT, param, 0x1, 10000) != HAL_OK)
  {
    Error_Handler();
  }
  /* A delay required for MFX to change IRQ_out pin config, before activate it on STM32L0 SW */
  HAL_Delay(1);
  
  /* IRQ_SRC: ERROR and IDD */
  param[0]=0x06;  
  if(HAL_I2C_Mem_Write(&I2CxHandle, 0x84, 0x42, I2C_MEMADD_SIZE_8BIT, param, 0x1, 10000) != HAL_OK)
  {
    Error_Handler();
  }
  
  /* Enable IDD function */
  param[0]=0x04;
  if(HAL_I2C_Mem_Write(&I2CxHandle, 0x84, 0x40, I2C_MEMADD_SIZE_8BIT, param, 0x1, 10000) != HAL_OK)
  {
    Error_Handler();
  }
  
  /* SH0 1000 mohm */
  param[0]=0x03; param[1]=0xE8; 
  /* SH1 24 ohm */
  param[2]=0x00; param[3]=0x18; 
  /* SH2 620 ohm */
  param[4]=0x02; param[5]=0x6C; 
  /* SH3 not exist with STM32L0538 discovery */
  param[6]=0x00; param[7]=0x00; 
  /* SH4 10000 ohm */
  param[8]=0x27; param[9]=0x10; 
  /* gain x49.9 (4990) */
  param[10]=0x13; param[11]=0x7E; 
  /* VDD_MCU min 3000mV (0xBB8) for undershoot detection */
  param[12]=0x0B; param[13]=0xB8; 
  /* Send 12 parameters */
  if(HAL_I2C_Mem_Write(&I2CxHandle, 0x84, 0x82, I2C_MEMADD_SIZE_8BIT, param, 0xE, 10000) != HAL_OK)  
  {
    Error_Handler();
  }

  /* Add a predelay before Idd measurement */
  param[0]=0xFF;
  if(HAL_I2C_Mem_Write(&I2CxHandle, 0x84, 0x81, I2C_MEMADD_SIZE_8BIT, param, 0x1, 10000) != HAL_OK)
  {
    Error_Handler();
  }

  /* Request IDD measurement with 4 shunts */
  param[0]=0x9;
  if(HAL_I2C_Mem_Write(&I2CxHandle, 0x84, 0x80, I2C_MEMADD_SIZE_8BIT, param, 0x1, 10000) != HAL_OK)
  {
    Error_Handler();
  }
 
  /* ############### System clock @ ~32KHz, MSI 65,5 KHz ##################### */

   /* The voltage scaling allows optimizing the power consumption when the device is 
     clocked below the maximum system frequency, to update the voltage scaling value 
     regarding system frequency refer to product datasheet. */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /* Enable MSI Oscillator */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_5;
  RCC_OscInitStruct.MSICalibrationValue = 0x00;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /* Select MSI as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  
  /* Enable MSI Oscillator */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_OFF;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /* Set MSI range to 0 */
  __HAL_RCC_MSI_RANGE_CONFIG(RCC_MSIRANGE_0);
  
  /* Enable Ultra low power mode */
  HAL_PWREx_EnableUltraLowPower();

  /* Enable the fast wake up from Ultra low power mode */
  HAL_PWREx_EnableFastWakeUp();

  /* Enable the power down mode during Sleep mode */
  __HAL_FLASH_SLEEP_POWERDOWN_ENABLE();

  /* Deinitialize the I2C */
  HAL_I2C_DeInit(&I2CxHandle);

   __HAL_RCC_GPIOC_CLK_ENABLE();
   /* Configure Button pin as input with External interrupt: Enable MFX IT generation on this SW */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH  ;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
  
  /* Enable and set Button EXTI Interrupt to the lowest priority */
  HAL_NVIC_SetPriority((IRQn_Type)(EXTI4_15_IRQn), 3, 0);
  HAL_NVIC_EnableIRQ((IRQn_Type)(EXTI4_15_IRQn));
  
  /*Suspend Tick increment to prevent wakeup by Systick interrupt. 
  Otherwise the Systick interrupt will wake up the device within 1ms (HAL time base)*/
  HAL_SuspendTick();
  
  /* Clear Wake Up Flag */
  __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);

  /* Enter Low Power Sleep Mode */
  HAL_PWR_EnterSLEEPMode(PWR_LOWPOWERREGULATOR_ON, PWR_SLEEPENTRY_WFI);

  /* Resume Tick interrupt if disabled prior to sleep mode entry*/
  HAL_ResumeTick();

  /* Configure the System Clock @ 16 Mhz afetr wakeup from LP Sleep mode */
  SystemClock_Config();

  /* Restore context */
  IDD_RestoreContext();
  
  /* Configure the I2C */
  I2C_Config();

  aRxBuffer[0] = 0x00;
  aRxBuffer[1] = 0x00;
  aRxBuffer[2] = 0x00;

  /* Idd measurement */
  if(HAL_I2C_Mem_Read(&I2CxHandle, 0x84, 0x14, I2C_MEMADD_SIZE_8BIT, (uint8_t*)aRxBuffer, 3, 10000) != HAL_OK)
  {
    Error_Handler();
  } 

  /* Acknowledge IDD IRQ from MFX */
  param[0]=0x2;
  if(HAL_I2C_Mem_Write(&I2CxHandle, 0x84, 0x44, I2C_MEMADD_SIZE_8BIT, param, 0x1, 10000) != HAL_OK)
  {
    Error_Handler();
  }
  /* delay for MFX to remove IRQ signal */
  HAL_Delay(1);
  
  /* Get the IDD measurement value */
  Idd_measurement = (aRxBuffer[0]<<16) + (aRxBuffer[1]<<8) + aRxBuffer[2];
  Idd_measurement = Idd_measurement * 10;
  sprintf((char*)str, "%d,%.3duA", (int)Idd_measurement/1000, ((int)Idd_measurement%1000));
}

/**
  * @brief  Process for measuring the IDD in Stop mode
  * @param  None
  * @retval None
  */
void Idd_Stop_process(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  /* Configure the I2C */
  I2C_Config();

  /* ######################### MFX Configuration ############################ */
  param[0]=0x80;
  if(HAL_I2C_Mem_Write(&I2CxHandle, 0x84, 0x40, I2C_MEMADD_SIZE_8BIT, param, 0x1, 10000) != HAL_OK)
  {
    Error_Handler();
  }
  /* Required delay to reset MFX */
  HAL_Delay(100);

  /* IRQ pin in push-pull, active high */
  param[0]=0x03;
  if(HAL_I2C_Mem_Write(&I2CxHandle, 0x84, 0x41, I2C_MEMADD_SIZE_8BIT, param, 0x1, 10000) != HAL_OK)
  {
    Error_Handler();
  }
  /* A delay required for MFX to change IRQ_out pin config, before activate it on STM32L0 SW */
  HAL_Delay(1);

  /* IRQ_SRC: ERROR and IDD */
  param[0]=0x06;  
  if(HAL_I2C_Mem_Write(&I2CxHandle, 0x84, 0x42, I2C_MEMADD_SIZE_8BIT, param, 0x1, 10000) != HAL_OK)
  {
    Error_Handler();
  }
  
  /* Enable IDD function */
  param[0]=0x04; 
  if(HAL_I2C_Mem_Write(&I2CxHandle, 0x84, 0x40, I2C_MEMADD_SIZE_8BIT, param, 0x1, 10000) != HAL_OK)
  {
    Error_Handler();
  }
  
  /* SH0 1000 mohm */
  param[0]=0x03; param[1]=0xE8; 
  /* SH1 24 ohm */
  param[2]=0x00; param[3]=0x18;
  /* SH2 620 ohm */
  param[4]=0x02; param[5]=0x6C; 
  /* SH3 not exist with STM32L053 discovery */
  param[6]=0x00; param[7]=0x00;
  /* SH4 10000 ohm */
  param[8]=0x27; param[9]=0x10;
  /* gain x49.9 (4990) */
  param[10]=0x13; param[11]=0x7E;
  /* VDD_MCU min 3000mV (0xBB8) for undershoot detection */
  param[12]=0x0B; param[13]=0xB8; 
  /* send 12 parameters */
  if(HAL_I2C_Mem_Write(&I2CxHandle, 0x84, 0x82, I2C_MEMADD_SIZE_8BIT, param, 0xE, 10000) != HAL_OK) 
  {
    Error_Handler();
  }

  /* Add a predelay before Idd measurement */
  param[0]=0xFF;
  if(HAL_I2C_Mem_Write(&I2CxHandle, 0x84, 0x81, I2C_MEMADD_SIZE_8BIT, param, 0x1, 10000) != HAL_OK)
  {
    Error_Handler();
  }

  /* Request IDD measurement with 4 shunts */
  param[0]=0x9;
  if(HAL_I2C_Mem_Write(&I2CxHandle, 0x84, 0x80, I2C_MEMADD_SIZE_8BIT, param, 0x1, 10000) != HAL_OK)
  {
    Error_Handler();
  }
  
  /* Save context */
  IDD_SaveContext();
  
  /* Deinitialize the I2C */
  HAL_I2C_DeInit(&I2CxHandle);
   
   __HAL_RCC_GPIOC_CLK_ENABLE();
   /* Configure Button pin as input with External interrupt: Enable MFX IT generation on this SW */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH  ;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
  
  /* Enable and set Button EXTI Interrupt to the lowest priority */
  HAL_NVIC_SetPriority((IRQn_Type)(EXTI4_15_IRQn), 3, 0);
  HAL_NVIC_EnableIRQ((IRQn_Type)(EXTI4_15_IRQn));
  
  /* Enable Ultra low power mode */
  HAL_PWREx_EnableUltraLowPower();
  
  /* Enable the fast wake up from Ultra low power mode */
  HAL_PWREx_EnableFastWakeUp();
  
  /* Clear Wake Up Flag */
  __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
    
  /* Enter Stop Mode */
  HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_SLEEPENTRY_WFI);
  
  /* Restore context */
  IDD_RestoreContext();
  
  /* Configure the I2C */
  I2C_Config();

  aRxBuffer[0] = 0x00;
  aRxBuffer[1] = 0x00;
  aRxBuffer[2] = 0x00;

  /* Idd measurement */
  if(HAL_I2C_Mem_Read(&I2CxHandle, 0x84, 0x14, I2C_MEMADD_SIZE_8BIT, (uint8_t*)aRxBuffer, 3, 10000) != HAL_OK)
  {
    Error_Handler();
  }

  /* Acknowledge IDD IRQ from MFX */
  param[0]=0x2;
  if(HAL_I2C_Mem_Write(&I2CxHandle, 0x84, 0x44, I2C_MEMADD_SIZE_8BIT, param, 0x1, 10000) != HAL_OK)
  {
    Error_Handler();
  }
  /* delay for MFX to remove IRQ signal */
  HAL_Delay(1);
  
  /* Get the IDD measurement value */
  Idd_measurement = (aRxBuffer[0]<<16) + (aRxBuffer[1]<<8) + aRxBuffer[2] ;
  Idd_measurement = Idd_measurement * 10;
  sprintf((char*)str, "%dnA", (int)Idd_measurement);
}

/**
  * @brief  I2C Configuration
  * @param  None
  * @retval None
  */
static void I2C_Config(void)
{
  HAL_I2C_DeInit(&I2CxHandle);
  /*##-1- Configure the I2C peripheral #######################################*/
  I2CxHandle.Instance              = I2C1;
  I2CxHandle.Init.AddressingMode   = I2C_ADDRESSINGMODE_7BIT;
  I2CxHandle.Init.Timing           = 0x00B1112E;
  I2CxHandle.Init.DualAddressMode  = I2C_DUALADDRESS_DISABLE;
  I2CxHandle.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  I2CxHandle.Init.GeneralCallMode  = I2C_GENERALCALL_DISABLE;
  I2CxHandle.Init.NoStretchMode    = I2C_NOSTRETCH_DISABLE;
  I2CxHandle.Init.OwnAddress1      = 0x0;
  I2CxHandle.Init.OwnAddress2      = 0x0;
  if(HAL_I2C_Init(&I2CxHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();    
  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  while(1)
  {
  }
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
