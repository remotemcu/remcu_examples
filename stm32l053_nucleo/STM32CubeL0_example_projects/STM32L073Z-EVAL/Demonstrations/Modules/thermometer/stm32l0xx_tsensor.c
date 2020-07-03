/**
  ******************************************************************************
  * @file    stm32l0xx_tsensor.c
  * @author  MCD Application Team
  * @brief   This file provides a set of functions needed to manage the SMBUS  
  *          temperature sensor (STLM75) mounted on STM32L073Z-EVAL board . 
  *          It implements a high level communication layer for initialized,
  *          read temperature or get the status of the temperature sensor.
  *          This driver implements too the SMBUS ALERT protocol.
  *     +--------------------------------------------------------------------+
  *     |                        Pin assignment                              |                 
  *     +----------------------------------------+--------------+------------+
  *     |  STM32L0xx SMBUS Pins                  |   STLM75     |   Pin      |
  *     +----------------------------------------+--------------+------------+
  *     | TSENSOR_SMBUS_SDA_PIN/ SDA             |   SDA        |    1        |
  *     | TSENSOR_SMBUS_SCL_PIN/ SCL             |   SCL        |    2        |
  *     | TSENSOR_SMBUS_ALERT_PIN/ SMBUS ALERT   |   OS/INT     |    3        |
  *     | .                                      |   GND        |    4  (0V)  |
  *     | .                                      |   GND        |    5  (0V)  |
  *     | .                                      |   GND        |    6  (0V)  |
  *     | .                                      |   GND        |    7  (0V)  |
  *     | .                                      |   VDD        |    8  (3.3V)|
  *     +----------------------------------------+--------------+------------+
  *      =================================================================== 
  *     Notes:
  *         - The Temperature Sensor (STLM75) is compatible
  *           with the SMBUS protocol.
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
#include "stm32l0xx_tsensor.h"

/** @addtogroup STM32L0xx_HAL_Examples
  * @{
  */

/** @addtogroup SMBUS_TSENSOR
  * @{
  */ 

/** @defgroup TSENSOR
  * @brief     This file provides a set of functions needed to drive the 
  *            Temperature Sensor STLM75.
  * @{
  */

/** @defgroup TSENSOR_Private_TypesDefinitions
  * @{
  */

/**
  * @}
  */

/** @defgroup TSENSOR_Private_Defines
  * @{
  */
/* TSENSOR TIMING Register define when SMBUS clock source is SYSCLK */
/* TSENSOR TIMING is calculated in case of the SMBUS Clock source is the SYSCLK = 32 MHz */
/* Set 0x10900CD6 value to reach 100 KHz speed (Rise time = 300ns, Fall time = 20ns) */
#define TSENSOR_TIMING              0x10900CD6
#define TSENSOR_SMBUS               I2C1
#define TSENSOR_TIMEOUT             1000
/**
  * @}
  */

/** @defgroup TSENSOR_Private_Macros
  * @{
  */

/**
  * @}
  */

/** @defgroup TSENSOR_Private_Variables
  * @{
  */ 
/* SMBUS handler declaration */
SMBUS_HandleTypeDef SmbusHandle;
/**
  * @}
  */

/** @defgroup TSENSOR_Private_FunctionPrototypes
  * @{
  */

/**
  * @}
  */

/** @defgroup TSENSOR_Private_Functions
  * @{
  */


/**
  * @brief  Set TSENSOR DeInitialization.
  * @param  None
  * @retval TSENSOR status
  */

TSENSOR_SMB_StatusTypeDef TSENSOR_DeInit()
{
   if(HAL_SMBUS_DeInit(&SmbusHandle) != HAL_OK)
  {
    return TSENSOR_SMB_ERROR;
  }
  return TSENSOR_SMB_OK;
}
/**
  * @brief  Set TSENSOR Initialization.
  * @param  DeviceAddr : Device ID address.
  * @param  pInitStruct: pointer to a TSENSOR_SMB_InitTypeDef structure 
  *         that contains the configuration setting for the TSENSOR.
  * @retval TSENSOR status
  */
TSENSOR_SMB_StatusTypeDef TSENSOR_Init(uint16_t DeviceAddr, TSENSOR_SMB_InitTypeDef *pInitStruct)
{  
  uint8_t buffertx[3] = {0,0,0};

  /*##-1- Initialise the SMBUS peripheral ####################################*/
  SmbusHandle.Instance                  = TSENSOR_SMBUS;
  
  SmbusHandle.Init.Timing               = TSENSOR_TIMING;
  SmbusHandle.Init.AnalogFilter         = SMBUS_ANALOGFILTER_ENABLE;
  SmbusHandle.Init.OwnAddress1          = 0x00;
  SmbusHandle.Init.AddressingMode       = SMBUS_ADDRESSINGMODE_7BIT;
  SmbusHandle.Init.DualAddressMode      = SMBUS_DUALADDRESS_DISABLE;
  SmbusHandle.Init.OwnAddress2          = 0x00;
  SmbusHandle.Init.GeneralCallMode      = SMBUS_GENERALCALL_DISABLE;
  SmbusHandle.Init.NoStretchMode        = SMBUS_NOSTRETCH_DISABLE;  
  SmbusHandle.Init.PacketErrorCheckMode = SMBUS_PEC_DISABLE;
  SmbusHandle.Init.PeripheralMode       = SMBUS_PERIPHERAL_MODE_SMBUS_HOST;
  SmbusHandle.Init.SMBusTimeout         = 0;
  
  if(HAL_SMBUS_Init(&SmbusHandle) != HAL_OK)
  {
    return TSENSOR_SMB_ERROR;
  }

  /*##-2- Verify that Temperature Sensor is ready ############################*/
  if (TSENSOR_IsReady(DeviceAddr, 20) != TSENSOR_SMB_OK )
  {
    return TSENSOR_SMB_ERROR;
  }

  /*##-3- Set the Configuration Register #####################################*/
  buffertx[0] = TSENSOR_REG_CONF;
  buffertx[1] = (uint8_t)(pInitStruct->AlertMode | pInitStruct->ConversionMode);
  if (HAL_SMBUS_Master_Transmit_IT(&SmbusHandle, DeviceAddr, &buffertx[0], /*sizeof(buffertx)*/2, SMBUS_FIRST_AND_LAST_FRAME_NO_PEC) != HAL_OK)
  {
    return TSENSOR_SMB_ERROR;
  }
  while(HAL_SMBUS_GetState(&SmbusHandle) != HAL_SMBUS_STATE_READY);
  
  /*##-4- Set the HIGH Limit Temperature #####################################*/
  buffertx[0] = TSENSOR_REG_TOS;
  buffertx[1] = (uint8_t)(pInitStruct->TemperatureLimitHigh);
  buffertx[2] = 0;

  if (HAL_SMBUS_Master_Transmit_IT(&SmbusHandle, DeviceAddr, &buffertx[0], /*sizeof(buffertx)*/3, SMBUS_FIRST_AND_LAST_FRAME_NO_PEC) != HAL_OK)
  {
    return TSENSOR_SMB_ERROR;
  }
  while(HAL_SMBUS_GetState(&SmbusHandle) != HAL_SMBUS_STATE_READY);

  /*##-5- Set the low Limit Temperature ######################################*/
  buffertx[0] = TSENSOR_REG_THYS;
  buffertx[1] = (uint8_t)(pInitStruct->TemperatureLimitLow);
  buffertx[2] = 0;
  
  if (HAL_SMBUS_Master_Transmit_IT(&SmbusHandle, DeviceAddr, &buffertx[0], /*sizeof(buffertx)*/3, SMBUS_FIRST_AND_LAST_FRAME_NO_PEC) != HAL_OK)
  {
    return TSENSOR_SMB_ERROR;
  }
  while(HAL_SMBUS_GetState(&SmbusHandle) != HAL_SMBUS_STATE_READY);
  
  /*##-6- Check Alert Mode requested #########################################*/
  if(pInitStruct->AlertMode == TSENSOR_INTERRUPT_MODE)
  {
    /* Enable SMBUS Alert */
    HAL_SMBUS_EnableAlert_IT(&SmbusHandle);
  }

  /* Return status OK*/
  return TSENSOR_SMB_OK;
}

/**
  * @brief  Checks if temperature sensor is ready for communication
  * @param  DeviceAddr : Device ID address.
  * @param  Trials: Number of trials
  * @retval TSENSOR status
  */
TSENSOR_SMB_StatusTypeDef TSENSOR_IsReady(uint16_t DeviceAddr, uint32_t Trials)
{
  /*##-1- Check is Temperature Sensor is Ready to use ########################*/
  if (HAL_SMBUS_IsDeviceReady(&SmbusHandle, DeviceAddr, Trials, TSENSOR_TIMEOUT) != HAL_OK)
  {
    return TSENSOR_SMB_ERROR;
  }
  
  /* Return status OK*/
  return TSENSOR_SMB_OK;
}

/**
  * @brief  Read The Temperature Sensor Status
  * @param  DeviceAddr : Device ID address.
  * @retval Temperature Sensor Status
  */
uint8_t TSENSOR_ReadStatus(uint16_t DeviceAddr)
{
  uint8_t bufferrx[1] = {0};
  uint8_t buffertx[1] = {0};

  /*##-1- Send Status Command ################################################*/
  buffertx[0] = TSENSOR_REG_CONF;
  if (HAL_SMBUS_Master_Transmit_IT(&SmbusHandle, DeviceAddr, &buffertx[0], sizeof(buffertx), SMBUS_FIRST_AND_LAST_FRAME_NO_PEC) != HAL_OK)
  {
    return TSENSOR_SMB_ERROR;
  }
  while(HAL_SMBUS_GetState(&SmbusHandle) != HAL_SMBUS_STATE_READY);
  
  /*##-2- Retrieve Status Data Byte ##########################################*/
  if (HAL_SMBUS_Master_Receive_IT(&SmbusHandle, DeviceAddr, &bufferrx[0], sizeof(buffertx), SMBUS_FIRST_AND_LAST_FRAME_NO_PEC) != HAL_OK)
  {
    return TSENSOR_SMB_ERROR;
  }
  while(HAL_SMBUS_GetState(&SmbusHandle) != HAL_SMBUS_STATE_READY);

  /* Return Temperature Sensor Status */
  return (uint8_t)(bufferrx[0]);
}

/**
  * @brief  Read temperature value of the Temperature Sensor.
  * @param  DeviceAddr : Device ID address.
  * @retval Temperature measured
  */
uint16_t TSENSOR_ReadTemp(uint16_t DeviceAddr)
{
  uint8_t bufferrx[2] = {0, 0};
  uint8_t buffertx[1] = {TSENSOR_REG_TEMP};
  uint16_t tmp = 0;

  /*##-1- Send Temperature Read Command ####################################*/
  if (HAL_SMBUS_Master_Transmit_IT(&SmbusHandle, DeviceAddr, &buffertx[0], 1, SMBUS_FIRST_FRAME) != HAL_OK)
  {
    return TSENSOR_SMB_ERROR;
  }
  while(HAL_SMBUS_GetState(&SmbusHandle) != HAL_SMBUS_STATE_READY);
  
  /*##-2- Retrieve Temperature Data Byte ###################################*/
  if (HAL_SMBUS_Master_Receive_IT(&SmbusHandle, DeviceAddr, &bufferrx[0], 2, SMBUS_LAST_FRAME_NO_PEC) != HAL_OK)
  {
    return TSENSOR_SMB_ERROR;
  }
  while(HAL_SMBUS_GetState(&SmbusHandle) != HAL_SMBUS_STATE_READY);

  tmp = (uint16_t)(bufferrx[0] << 8);
  tmp |= bufferrx[1];
  
  /* Return Temperature value */
  return (uint16_t)(tmp >> 4);
}

/**
  * @brief  Get the address of the Temperature Sensor in alert mode.
  * @param  None
  * @retval Temperature Sensor Address.
  */
uint8_t TSENSOR_SMB_ALERTeResponseAddressRead(void)
{
  uint8_t bufferrx[1] = {0};

  /*##-1- Retrieve Alert Temperature Sensor Address ##########################*/
  HAL_SMBUS_Master_Receive_IT(&SmbusHandle, 0x18, &bufferrx[0], 1, SMBUS_FIRST_AND_LAST_FRAME_NO_PEC);  
  while(HAL_SMBUS_GetState(&SmbusHandle) != HAL_SMBUS_STATE_READY);
  
  /* Return Temperature Sensor Address */
  return bufferrx[0];
}

/**
  * @brief  SMBUS error callbacks.
  * @param  hsmbus : Pointer to a SMBUS_HandleTypeDef structure that contains
  *                the configuration information for the specified SMBUS.
  * @retval None
  */
void HAL_SMBUS_ErrorCallback(SMBUS_HandleTypeDef *hsmbus)
{
  if(HAL_SMBUS_GetError(hsmbus) == HAL_SMBUS_ERROR_ALERT)
  {
    /* Inform upper layer of an alert occurs */
    TSENSOR_SMB_ERRORCallback(TSENSOR_SMB_ALERT);
  }
  else
  {
    /* Inform upper layer of an error occurs */
    TSENSOR_SMB_ERRORCallback(TSENSOR_SMB_ERROR);
  }
  
}

/**
  * @brief  TSENSOR error callbacks.
  * @param  Error : Temperature Sensor Error status.
  * @retval None
  */
__weak void TSENSOR_SMB_ERRORCallback(uint16_t Error)
{
  /* NOTE : This function Should not be modified, when the callback is needed,
            the TSENSOR_SMB_ERRORCallback could be implemented in the user file
   */ 
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/     
