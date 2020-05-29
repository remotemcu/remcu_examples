/**
  ******************************************************************************
  * @file    stm32f3_discovery_lsm303dlhc.c
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    20-September-2012
  * @brief   This file provides a set of functions needed to manage the lsm303dlhc
  *          MEMS accelerometer available on STM32F3-Discovery Kit.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
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
#include "stm32f3_discovery_lsm303dlhc.h"

/** @addtogroup Utilities
  * @{
  */ 

/** @addtogroup STM32F3_DISCOVERY
  * @{
  */ 

/** @addtogroup STM32F3_DISCOVERY_LSM303DLHC
  * @{
  */


/** @defgroup STM32F3_DISCOVERY_LSM303DLHC_Private_TypesDefinitions
  * @{
  */

/**
  * @}
  */

/** @defgroup STM32F3_DISCOVERY_LSM303DLHC_Private_Defines
  * @{
  */

/**
  * @}
  */

/** @defgroup STM32F3_DISCOVERY_LSM303DLHC_Private_Macros
  * @{
  */

/**
  * @}
  */ 
  
/** @defgroup STM32F3_DISCOVERY_LSM303DLHC_Private_Variables
  * @{
  */ 
__IO uint32_t  LSM303DLHC_Timeout = LSM303DLHC_LONG_TIMEOUT; 
/**
  * @}
  */

/** @defgroup STM32F3_DISCOVERY_LSM303DLHC_Private_FunctionPrototypes
  * @{
  */
static void LSM303DLHC_LowLevel_Init(void);
/**
  * @}
  */

/** @defgroup STM32F3_DISCOVERY_LSM303DLHC_Private_Functions
  * @{
  */

/**
  * @brief  Set LSM303DLHC Initialization.
  * @param  LSM303DLHC_InitStruct: pointer to a LSM303DLHC_InitTypeDef structure 
  *         that contains the configuration setting for the LSM303DLHC.
  * @retval None
  */
void LSM303DLHC_AccInit(LSM303DLHCAcc_InitTypeDef *LSM303DLHC_InitStruct)
{  
  uint8_t ctrl1 = 0x00, ctrl4 = 0x00;
  
  /* Configure the low level interface ---------------------------------------*/
  LSM303DLHC_LowLevel_Init();
  
  /* Configure MEMS: data rate, power mode, full scale and axes */
  ctrl1 |= (uint8_t) (LSM303DLHC_InitStruct->Power_Mode | LSM303DLHC_InitStruct->AccOutput_DataRate | \
                    LSM303DLHC_InitStruct->Axes_Enable);
  
  ctrl4 |= (uint8_t) (LSM303DLHC_InitStruct->BlockData_Update | LSM303DLHC_InitStruct->Endianness | \
                    LSM303DLHC_InitStruct->AccFull_Scale|LSM303DLHC_InitStruct->High_Resolution);
                    
  /* Write value to ACC MEMS CTRL_REG1 regsister */
  LSM303DLHC_Write(ACC_I2C_ADDRESS, LSM303DLHC_CTRL_REG1_A, &ctrl1);
  
  /* Write value to ACC MEMS CTRL_REG4 regsister */
  LSM303DLHC_Write(ACC_I2C_ADDRESS, LSM303DLHC_CTRL_REG4_A, &ctrl4);
}

/**
  * @brief  Reboot memory content of LSM303DLHC
  * @param  None
  * @retval None
  */
void LSM303DLHC_AccRebootCmd(void)
{
  uint8_t tmpreg;
  
  /* Read CTRL_REG5 register */
  LSM303DLHC_Read(ACC_I2C_ADDRESS, LSM303DLHC_CTRL_REG5_A, &tmpreg, 1);
  
  /* Enable or Disable the reboot memory */
  tmpreg |= LSM303DLHC_BOOT_REBOOTMEMORY;
  
  /* Write value to ACC MEMS CTRL_REG5 regsister */
  LSM303DLHC_Write(ACC_I2C_ADDRESS, LSM303DLHC_CTRL_REG5_A, &tmpreg);
}

/**
  * @brief  Set High Pass Filter Modality
  * @param  LSM303DLHC_FilterStruct: pointer to a LSM303DLHC_FilterConfigTypeDef structure 
  *         that contains the configuration setting for the LSM303DLHC.        
  * @retval None
  */
void LSM303DLHC_AccFilterConfig(LSM303DLHCAcc_FilterConfigTypeDef *LSM303DLHC_FilterStruct) 
{
  uint8_t tmpreg;
  
  /* Read CTRL_REG2 register */
  LSM303DLHC_Read(ACC_I2C_ADDRESS, LSM303DLHC_CTRL_REG2_A, &tmpreg, 1);
  
  tmpreg &= 0x0C;
  
  /* Configure MEMS: mode, cutoff frquency, Filter status, Click, AOI1 and AOI2 */
  tmpreg |= (uint8_t) (LSM303DLHC_FilterStruct->HighPassFilter_Mode_Selection |\
                      LSM303DLHC_FilterStruct->HighPassFilter_CutOff_Frequency|\
                      LSM303DLHC_FilterStruct->HighPassFilter_AOI1|\
                      LSM303DLHC_FilterStruct->HighPassFilter_AOI2);                             
  
  /* Write value to ACC MEMS CTRL_REG2 regsister */
  LSM303DLHC_Write(ACC_I2C_ADDRESS, LSM303DLHC_CTRL_REG2_A, &tmpreg);
}

/**
  * @brief  Enable or Disable High Pass Filter
  * @param  HighPassFilterState: new state of the High Pass Filter feature.
  *      This parameter can be: 
  *         @arg: LSM303DLHC_HighPassFilter_DISABLE 
  *         @arg: LSM303DLHC_HighPassFilter_ENABLE          
  * @retval None
  */
void LSM303DLHC_AccFilterCmd(uint8_t HighPassFilterState)
 {
  uint8_t tmpreg;
  
  /* Read CTRL_REG2 register */
  LSM303DLHC_Read(ACC_I2C_ADDRESS, LSM303DLHC_CTRL_REG2_A, &tmpreg, 1);
                  
  tmpreg &= 0xF7;

  tmpreg |= HighPassFilterState;

  /* Write value to ACC MEMS CTRL_REG2 regsister */
  LSM303DLHC_Write(ACC_I2C_ADDRESS, LSM303DLHC_CTRL_REG2_A, &tmpreg);
}

/**
  * @brief  Enable or Disable High Pass Filter on CLick
  * @param  HighPassFilterState: new state of the High Pass Filter feature.
  *      This parameter can be: 
  *         @arg: LSM303DLHC_HighPassFilter_DISABLE 
  *         @arg: LSM303DLHC_HighPassFilter_ENABLE          
  * @retval None
  */
void LSM303DLHC_AccFilterClickCmd(uint8_t HighPassFilterClickState)
 {
  uint8_t tmpreg;
  
  /* Read CTRL_REG2 register */
  LSM303DLHC_Read(ACC_I2C_ADDRESS, LSM303DLHC_CTRL_REG2_A, &tmpreg, 1);
                  
  tmpreg &= 0xFB;

  tmpreg |= HighPassFilterClickState;

  /* Write value to ACC MEMS CTRL_REG2 regsister */
  LSM303DLHC_Write(ACC_I2C_ADDRESS, LSM303DLHC_CTRL_REG2_A, &tmpreg);
}

/**
  * @brief Set LSM303DLHC Interrupt1 configuration
  * @param  LSM303DLHC_IT: specifies the LSM303DLHC interrupt source to be enabled.
  *           This parameter can be any combination of the following values: 
  *         @arg   LSM303DLHC_IT1_CLICK
  *         @arg   LSM303DLHC_IT1_AOI1
  *         @arg   LSM303DLHC_IT1_AOI2
  *         @arg   LSM303DLHC_IT1_DRY1
  *         @arg   LSM303DLHC_IT1_WTM
  *         @arg   LSM303DLHC_IT1_OVERRUN              
  * @param  NewState: new state of the selected LSM303DLHC interrupts.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void LSM303DLHC_AccIT1Config(uint8_t LSM303DLHC_IT, FunctionalState NewState)
{
  uint8_t tmpval = 0x00;
  
  /* Read CTRL_REG3 register */
  LSM303DLHC_Read(ACC_I2C_ADDRESS, LSM303DLHC_CTRL_REG3_A, &tmpval, 1 );
    
  tmpval &= ~LSM303DLHC_IT;
  
  if (NewState != DISABLE)
  {
    tmpval |= LSM303DLHC_IT;
  }
  else
  {
    /* Disable the selected interrupt */
    tmpval =~ LSM303DLHC_IT;
  }
      
  /* Write value to MEMS CTRL_REG3 register */
  LSM303DLHC_Write(ACC_I2C_ADDRESS, LSM303DLHC_CTRL_REG3_A, &tmpval);  
}

/**
  * @brief Set LSM303DLHC Interrupt2 configuration
  * @param  LSM303DLHC_IT: specifies the LSM303DLHC interrupt source to be enabled.
  *           This parameter can be any combination of the following values: 
  *         @arg   LSM303DLHC_IT2_CLICK2
  *         @arg   LSM303DLHC_IT2_INT1
  *         @arg   LSM303DLHC_IT2_INT2
  *         @arg   LSM303DLHC_IT2_BOOT
  *         @arg   LSM303DLHC_IT2_ACT
  *         @arg   LSM303DLHC_IT2_HLACTIVE              
  * @param  NewState: new state of the selected LSM303DLHC interrupts.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void LSM303DLHC_AccIT2Config(uint8_t LSM303DLHC_IT, FunctionalState NewState)
{
  uint8_t tmpval = 0x00;
  
  /* Read CTRL_REG3 register */
  LSM303DLHC_Read(ACC_I2C_ADDRESS, LSM303DLHC_CTRL_REG6_A, &tmpval, 1);
    
  tmpval &= ~LSM303DLHC_IT;
  
  if (NewState != DISABLE)
  {
    tmpval |= LSM303DLHC_IT;
  }
  else
  {
    /* Disable the selected interrupt */
    tmpval =~ LSM303DLHC_IT;
  }
      
  /* Write value to MEMS CTRL_REG3 register */
  LSM303DLHC_Write(ACC_I2C_ADDRESS, LSM303DLHC_CTRL_REG6_A, &tmpval);  
}

/**
  * @brief  INT1 interrupt config
  * @param  ITCombination: Or or And combination
  *         ITAxes: axes to be enabled 
  *         NewState: Enable or Disable    
  * @retval None
  */
void LSM303DLHC_AccINT1InterruptConfig(uint8_t ITCombination, uint8_t ITAxes, FunctionalState NewState )
{  
  uint8_t tmpval = ITCombination;
  
  /* Read INT1_CFR register */
  LSM303DLHC_Read(ACC_I2C_ADDRESS, LSM303DLHC_INT1_CFG_A, &tmpval, 1);
  
  if (NewState != DISABLE)
  {
    tmpval |= ITAxes;
  }
  else
  {
    /* Disable the selected interrupt */
    tmpval =(~ITAxes)|ITCombination;
  }
      
  /* Write value to MEMS INT1_CFR register */
  LSM303DLHC_Write(ACC_I2C_ADDRESS, LSM303DLHC_INT1_CFG_A, &tmpval);  
}

/**
  * @brief  INT1 interrupt config
  * @param  ITCombination: Or or And combination
  *         ITAxes: axes to be enabled 
  *         NewState: Enable or Disable    
  * @retval None
  */
void LSM303DLHC_AccINT2InterruptConfig(uint8_t ITCombination, uint8_t ITAxes, FunctionalState NewState )
{  
  uint8_t tmpval = ITCombination;
  
  /* Read INT2_CFR register */
  LSM303DLHC_Read(ACC_I2C_ADDRESS, LSM303DLHC_INT2_CFG_A, &tmpval, 1);
  
  if (NewState != DISABLE)
  {
    tmpval |= ITAxes;
  }
  else
  {
    /* Disable the selected interrupt */
    tmpval =(~ITAxes)|ITCombination;
  }
      
  /* Write value to MEMS INT2_CFR register */
  LSM303DLHC_Write(ACC_I2C_ADDRESS, LSM303DLHC_INT2_CFG_A, &tmpval);  
}

/**
  * @brief  INT1 interrupt config
  * @param  ITCombination: Or or And combination
  *         ITAxes: axes to be enabled 
  *         NewState: Enable or Disable    
  * @retval None
  */
void LSM303DLHC_AccClickITConfig(uint8_t ITClick, FunctionalState NewState)
{  
  uint8_t tmpval;
  
  /* Read CLICK_CFR register */
  LSM303DLHC_Read(ACC_I2C_ADDRESS, LSM303DLHC_CLICK_CFG_A, &tmpval, 1);
  
  if (NewState != DISABLE)
  {
    tmpval |= ITClick;
  }
  else
  {
    /* Disable the selected interrupt */
    tmpval =~ITClick;
  }
      
  /* Write value to MEMS CLICK_CFR register */
  LSM303DLHC_Write(ACC_I2C_ADDRESS, LSM303DLHC_CLICK_CFG_A, &tmpval);  
}

/**
  * @brief  Get status for Acc LSM303DLHC data
  * @param  None         
  * @retval Data status in a LSM303DLHC Data register
  */
uint8_t LSM303DLHC_AccGetDataStatus(void)
{
  uint8_t tmpreg;
  
  /* Read Mag STATUS register */
  LSM303DLHC_Read(ACC_I2C_ADDRESS, LSM303DLHC_STATUS_REG_A, &tmpreg, 1);
                  
  return tmpreg;
}

/**
  * @brief  Set LSM303DLHC Mag Initialization.
  * @param  LSM303DLHC_InitStruct: pointer to a LSM303DLHC_MagInitTypeDef structure 
  *         that contains the configuration setting for the LSM303DLHC.
  * @retval None
  */
void LSM303DLHC_MagInit(LSM303DLHCMag_InitTypeDef *LSM303DLHC_InitStruct)
{  
  uint8_t cra_regm = 0x00, crb_regm = 0x00, mr_regm = 0x00;
  
  /* Configure the low level interface ---------------------------------------*/
  LSM303DLHC_LowLevel_Init();
  
  /* Configure MEMS: temp and Data rate */
  cra_regm |= (uint8_t) (LSM303DLHC_InitStruct->Temperature_Sensor | LSM303DLHC_InitStruct->MagOutput_DataRate);
    
  /* Configure MEMS: full Scale */
  crb_regm |= (uint8_t) (LSM303DLHC_InitStruct->MagFull_Scale);
      
  /* Configure MEMS: working mode */
  mr_regm |= (uint8_t) (LSM303DLHC_InitStruct->Working_Mode);
                    
  /* Write value to Mag MEMS CRA_REG regsister */
  LSM303DLHC_Write(MAG_I2C_ADDRESS, LSM303DLHC_CRA_REG_M, &cra_regm);
  
  /* Write value to Mag MEMS CRB_REG regsister */
  LSM303DLHC_Write(MAG_I2C_ADDRESS, LSM303DLHC_CRB_REG_M, &crb_regm);

  /* Write value to Mag MEMS MR_REG regsister */
  LSM303DLHC_Write(MAG_I2C_ADDRESS, LSM303DLHC_MR_REG_M, &mr_regm);
}

/**
  * @brief  Get status for Mag LSM303DLHC data
  * @param  None         
  * @retval Data status in a LSM303DLHC Data register
  */
uint8_t LSM303DLHC_MagGetDataStatus(void)
{
  uint8_t tmpreg;
  
  /* Read Mag STATUS register */
  LSM303DLHC_Read(MAG_I2C_ADDRESS, LSM303DLHC_SR_REG_M, &tmpreg, 1 );
                  
  return tmpreg;
}

/**
  * @brief  Writes one byte to the LSM303DLHC.
  * @param  DeviceAddr : specifies the slave address to be programmed.
  * @param  RegAddr : specifies the LSM303DLHC register to be written.
  * @param  pBuffer : pointer to the buffer  containing the data to be written to the LSM303DLH.
  * @retval LSM303DLHC Status
  */
uint16_t LSM303DLHC_Write(uint8_t DeviceAddr, uint8_t RegAddr, uint8_t* pBuffer)
{  
  /* Test on BUSY Flag */
  LSM303DLHC_Timeout = LSM303DLHC_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(LSM303DLHC_I2C, I2C_ISR_BUSY) != RESET)
  {
    if((LSM303DLHC_Timeout--) == 0) return LSM303DLHC_TIMEOUT_UserCallback();
  }
  
  /* Configure slave address, nbytes, reload, end mode and start or stop generation */
  I2C_TransferHandling(LSM303DLHC_I2C, DeviceAddr, 1, I2C_Reload_Mode, I2C_Generate_Start_Write);
  
  /* Wait until TXIS flag is set */
  LSM303DLHC_Timeout = LSM303DLHC_LONG_TIMEOUT;  
  while(I2C_GetFlagStatus(LSM303DLHC_I2C, I2C_ISR_TXIS) == RESET)   
  {
    if((LSM303DLHC_Timeout--) == 0) return LSM303DLHC_TIMEOUT_UserCallback();
  }
  
  /* Send Register address */
  I2C_SendData(LSM303DLHC_I2C, (uint8_t) RegAddr);
  
  /* Wait until TCR flag is set */
  LSM303DLHC_Timeout = LSM303DLHC_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(LSM303DLHC_I2C, I2C_ISR_TCR) == RESET)
  {
    if((LSM303DLHC_Timeout--) == 0) return LSM303DLHC_TIMEOUT_UserCallback();
  }
  
  /* Configure slave address, nbytes, reload, end mode and start or stop generation */
  I2C_TransferHandling(LSM303DLHC_I2C, DeviceAddr, 1, I2C_AutoEnd_Mode, I2C_No_StartStop);
       
  /* Wait until TXIS flag is set */
  LSM303DLHC_Timeout = LSM303DLHC_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(LSM303DLHC_I2C, I2C_ISR_TXIS) == RESET)
  {
    if((LSM303DLHC_Timeout--) == 0) return LSM303DLHC_TIMEOUT_UserCallback();
  }  
    
  /* Write data to TXDR */
  I2C_SendData(LSM303DLHC_I2C, *pBuffer);
      
  /* Wait until STOPF flag is set */
  LSM303DLHC_Timeout = LSM303DLHC_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(LSM303DLHC_I2C, I2C_ISR_STOPF) == RESET)
  {
    if((LSM303DLHC_Timeout--) == 0) return LSM303DLHC_TIMEOUT_UserCallback();
  }   
  
  /* Clear STOPF flag */
  I2C_ClearFlag(LSM303DLHC_I2C, I2C_ICR_STOPCF);
  
  return LSM303DLHC_OK;
}

/**
  * @brief  Reads a block of data from the LSM303DLHC.
  * @param  DeviceAddr : specifies the slave address to be programmed(ACC_I2C_ADDRESS or MAG_I2C_ADDRESS).
  * @param  RegAddr : specifies the LSM303DLHC internal address register to read from.
  * @param  pBuffer : pointer to the buffer that receives the data read from the LSM303DLH.
  * @param  NumByteToRead : number of bytes to read from the LSM303DLH ( NumByteToRead >1  only for the Mgnetometer readinf).
  * @retval LSM303DLHC register value
  */
uint16_t LSM303DLHC_Read(uint8_t DeviceAddr, uint8_t RegAddr, uint8_t* pBuffer, uint16_t NumByteToRead)
{    
  /* Test on BUSY Flag */
  LSM303DLHC_Timeout = LSM303DLHC_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(LSM303DLHC_I2C, I2C_ISR_BUSY) != RESET)
  {
      if((LSM303DLHC_Timeout--) == 0) return (uint16_t)LSM303DLHC_TIMEOUT_UserCallback();
  }
  
  /* Configure slave address, nbytes, reload, end mode and start or stop generation */
  I2C_TransferHandling(LSM303DLHC_I2C, DeviceAddr, 1, I2C_SoftEnd_Mode, I2C_Generate_Start_Write);
  
  /* Wait until TXIS flag is set */
  LSM303DLHC_Timeout = LSM303DLHC_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(LSM303DLHC_I2C, I2C_ISR_TXIS) == RESET)
  {
    if((LSM303DLHC_Timeout--) == 0) return LSM303DLHC_TIMEOUT_UserCallback();
  }
  
  if(NumByteToRead>1)
      RegAddr |= 0x80;

  
  /* Send Register address */
  I2C_SendData(LSM303DLHC_I2C, (uint8_t)RegAddr);
  
  /* Wait until TC flag is set */
  LSM303DLHC_Timeout = LSM303DLHC_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(LSM303DLHC_I2C, I2C_ISR_TC) == RESET)
  {
    if((LSM303DLHC_Timeout--) == 0) return LSM303DLHC_TIMEOUT_UserCallback();
  }  
  
  /* Configure slave address, nbytes, reload, end mode and start or stop generation */
  I2C_TransferHandling(LSM303DLHC_I2C, DeviceAddr, NumByteToRead, I2C_AutoEnd_Mode, I2C_Generate_Start_Read);
  
  /* Wait until all data are received */
  while (NumByteToRead)
  {   
    /* Wait until RXNE flag is set */
    LSM303DLHC_Timeout = LSM303DLHC_LONG_TIMEOUT;
    while(I2C_GetFlagStatus(LSM303DLHC_I2C, I2C_ISR_RXNE) == RESET)    
    {
      if((LSM303DLHC_Timeout--) == 0) return LSM303DLHC_TIMEOUT_UserCallback();
    }
    
    /* Read data from RXDR */
    *pBuffer = I2C_ReceiveData(LSM303DLHC_I2C);
    /* Point to the next location where the byte read will be saved */
    pBuffer++;
    
    /* Decrement the read bytes counter */
    NumByteToRead--;
  } 
  
  /* Wait until STOPF flag is set */
  LSM303DLHC_Timeout = LSM303DLHC_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(LSM303DLHC_I2C, I2C_ISR_STOPF) == RESET)   
  {
    if((LSM303DLHC_Timeout--) == 0) return LSM303DLHC_TIMEOUT_UserCallback();
  }
  
  /* Clear STOPF flag */
  I2C_ClearFlag(LSM303DLHC_I2C, I2C_ICR_STOPCF);
  
  /* If all operations OK */
  return LSM303DLHC_OK;  
}  
/**
* @brief  Initializes the low level interface used to drive the LSM303DLHC
* @param  None
* @retval None
*/
static void LSM303DLHC_LowLevel_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  EXTI_InitTypeDef EXTI_InitStructure;
  I2C_InitTypeDef  I2C_InitStructure;
  
  /* Enable the I2C periph */
  RCC_APB1PeriphClockCmd(LSM303DLHC_I2C_CLK, ENABLE);
  
  /* Enable SCK and SDA GPIO clocks */
  RCC_AHBPeriphClockCmd(LSM303DLHC_I2C_SCK_GPIO_CLK | LSM303DLHC_I2C_SDA_GPIO_CLK , ENABLE);
  
  /* Enable INT1 GPIO clock */
  RCC_AHBPeriphClockCmd(LSM303DLHC_I2C_INT1_GPIO_CLK, ENABLE);
  
  /* Enable INT2 GPIO clock */
  RCC_AHBPeriphClockCmd(LSM303DLHC_I2C_INT2_GPIO_CLK, ENABLE);
  
  /* Enable DRDY clock */
  RCC_AHBPeriphClockCmd(LSM303DLHC_DRDY_GPIO_CLK, ENABLE);
  
  GPIO_PinAFConfig(LSM303DLHC_I2C_SCK_GPIO_PORT, LSM303DLHC_I2C_SCK_SOURCE, LSM303DLHC_I2C_SCK_AF);
  GPIO_PinAFConfig(LSM303DLHC_I2C_SDA_GPIO_PORT, LSM303DLHC_I2C_SDA_SOURCE, LSM303DLHC_I2C_SDA_AF);
  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  
  /* I2C SCK pin configuration */
  GPIO_InitStructure.GPIO_Pin = LSM303DLHC_I2C_SCK_PIN;
  GPIO_Init(LSM303DLHC_I2C_SCK_GPIO_PORT, &GPIO_InitStructure);
  
  /* I2C SDA pin configuration */
  GPIO_InitStructure.GPIO_Pin =  LSM303DLHC_I2C_SDA_PIN;
  GPIO_Init(LSM303DLHC_I2C_SDA_GPIO_PORT, &GPIO_InitStructure);
  
  /* Mems DRDY */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  
  /* Mems DRDY pin configuration */
  GPIO_InitStructure.GPIO_Pin = LSM303DLHC_DRDY_PIN;
  GPIO_Init(LSM303DLHC_DRDY_GPIO_PORT, &GPIO_InitStructure);
  
  /* Connect EXTI Line to Mems DRDY Pin */
  SYSCFG_EXTILineConfig(LSM303DLHC_DRDY_EXTI_PORT_SOURCE, LSM303DLHC_DRDY_EXTI_PIN_SOURCE);
  
  EXTI_InitStructure.EXTI_Line = LSM303DLHC_DRDY_EXTI_LINE;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  
  EXTI_Init(&EXTI_InitStructure);
  
  /* I2C configuration -------------------------------------------------------*/
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStructure.I2C_AnalogFilter = I2C_AnalogFilter_Enable;
  I2C_InitStructure.I2C_DigitalFilter = 0x00;
  I2C_InitStructure.I2C_OwnAddress1 = 0x00;
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_InitStructure.I2C_Timing = 0x00902025;
  
  /* Apply LSM303DLHC_I2C configuration after enabling it */
  I2C_Init(LSM303DLHC_I2C, &I2C_InitStructure);
  
  /* LSM303DLHC_I2C Peripheral Enable */
  I2C_Cmd(LSM303DLHC_I2C, ENABLE);
  
  /* Configure GPIO PINs to detect Interrupts */
  GPIO_InitStructure.GPIO_Pin = LSM303DLHC_I2C_INT1_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(LSM303DLHC_I2C_INT1_GPIO_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = LSM303DLHC_I2C_INT2_PIN;
  GPIO_Init(LSM303DLHC_I2C_INT2_GPIO_PORT, &GPIO_InitStructure);
}

/**
  * @brief  Basic management of the timeout situation.
  * @param  None.
  * @retval None.
  */

#ifdef USE_DEFAULT_TIMEOUT_CALLBACK
/**
  * @brief  Basic management of the timeout situation.
  * @param  None.
  * @retval None.
  */
uint32_t LSM303DLHC_TIMEOUT_UserCallback(void)
{
  /* Block communication and all processes */
  while (1)
  {   
  }
}
#endif /* USE_DEFAULT_TIMEOUT_CALLBACK */

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
