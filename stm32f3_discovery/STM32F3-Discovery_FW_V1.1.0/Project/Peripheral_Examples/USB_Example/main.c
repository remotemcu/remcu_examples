/**
  ******************************************************************************
  * @file    USB_Example/main.c  
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    20-September-2012
  * @brief   Main program body
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
#include "main.h"

/** @addtogroup STM32F3_Discovery_Peripheral_Examples
  * @{
  */

/** @addtogroup USB_Example
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define PI                         (float)     3.14159265f

#define LSM_Acc_Sensitivity_2g     (float)     1.0f            /*!< accelerometer sensitivity with 2 g full scale [LSB/mg] */
#define LSM_Acc_Sensitivity_4g     (float)     0.5f            /*!< accelerometer sensitivity with 4 g full scale [LSB/mg] */
#define LSM_Acc_Sensitivity_8g     (float)     0.25f           /*!< accelerometer sensitivity with 8 g full scale [LSB/mg] */
#define LSM_Acc_Sensitivity_16g    (float)     0.0834f         /*!< accelerometer sensitivity with 12 g full scale [LSB/mg] */

/* Private variables ---------------------------------------------------------*/
RCC_ClocksTypeDef RCC_Clocks;
__IO uint32_t TimingDelay = 0;

float  AccBuffer[3] = {0.0f};

__IO uint8_t DataReady = 0;
__IO uint8_t PrevXferComplete = 1;
uint8_t *Mouse_Buffer;

/* Private function prototypes -----------------------------------------------*/
static uint8_t *USBD_HID_GetPos (void);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None 
  * @retval None
  */
int main(void)
{  
  uint8_t i = 0;
  /* SysTick end of count event each 10ms */
  RCC_GetClocksFreq(&RCC_Clocks);
  SysTick_Config(RCC_Clocks.HCLK_Frequency / 100);
    
  /* Configure the USB */
  USB_Config();
  
  /* Accelerometer Configuration */
  Acc_Config();
  
  /* Infinite loop */
  while (1)
  {      
    
    /* Wait for data ready */
    while(DataReady !=0x04)
    {}
    DataReady = 0x00;
    
    /* Get Data Accelerometer */
    Acc_ReadData(AccBuffer);
    
    for(i=0;i<3;i++)
      AccBuffer[i] /= 100.0f;
    
    /* Get position */
    Mouse_Buffer = USBD_HID_GetPos();
    /* Update the cursor position */
    if((Mouse_Buffer[1] != 0) ||(Mouse_Buffer[2] != 0))
    {
      /* Reset the control token to inform upper layer that a transfer is ongoing */
      PrevXferComplete = 0;
      
      /* Copy mouse position info in ENDP1 Tx Packet Memory Area*/
      USB_SIL_Write(EP1_IN, Mouse_Buffer, 4);
      
      /* Enable endpoint for transmission */
      SetEPTxValid(ENDP1);
    }
  }
}

/**
  * @brief  Configure the USB.
  * @param  None
  * @retval None
  */
void USB_Config(void)
{
  Set_System();
  Set_USBClock();
  USB_Interrupts_Config();
  
  USB_Init();

  while (bDeviceState != CONFIGURED)
  {}
}

/**
  * @brief  USBD_HID_GetPos
  * @param  None
  * @retval Pointer to report
  */
static uint8_t *USBD_HID_GetPos (void)
{
  static uint8_t HID_Buffer[4] = {0};
  
  HID_Buffer[1] = 0;
  HID_Buffer[2] = 0;
  /* LEFT Direction */
  if(((int8_t)AccBuffer[1]) < -2)
  {
    HID_Buffer[1] += CURSOR_STEP;
  }
  /* RIGHT Direction */ 
  if(((int8_t)AccBuffer[1]) > 2)
  {
   HID_Buffer[1] -= CURSOR_STEP;
  } 
  /* UP Direction */
  if(((int8_t)AccBuffer[0]) < -2)
  {
    HID_Buffer[2] += CURSOR_STEP;
  }
  /* DOWN Direction */ 
  if(((int8_t)AccBuffer[0]) > 2)
  {
    HID_Buffer[2] -= CURSOR_STEP;
  } 
  
  return HID_Buffer;
}

/**
  * @brief  Configure the Mems to Accelerometer MEMS.
  * @param  None
  * @retval None
  */
void Acc_Config(void)
{
  LSM303DLHCAcc_InitTypeDef LSM303DLHCAcc_InitStructure;
  LSM303DLHCAcc_FilterConfigTypeDef LSM303DLHCFilter_InitStructure;
    
   /* Fill the accelerometer structure */
  LSM303DLHCAcc_InitStructure.Power_Mode = LSM303DLHC_NORMAL_MODE;
  LSM303DLHCAcc_InitStructure.AccOutput_DataRate = LSM303DLHC_ODR_50_HZ;
  LSM303DLHCAcc_InitStructure.Axes_Enable= LSM303DLHC_AXES_ENABLE;
  LSM303DLHCAcc_InitStructure.AccFull_Scale = LSM303DLHC_FULLSCALE_2G;
  LSM303DLHCAcc_InitStructure.BlockData_Update = LSM303DLHC_BlockUpdate_Continous;
  LSM303DLHCAcc_InitStructure.Endianness=LSM303DLHC_BLE_LSB;
  LSM303DLHCAcc_InitStructure.High_Resolution=LSM303DLHC_HR_ENABLE;
  /* Configure the accelerometer main parameters */
  LSM303DLHC_AccInit(&LSM303DLHCAcc_InitStructure);
  
  /* Fill the accelerometer LPF structure */
  LSM303DLHCFilter_InitStructure.HighPassFilter_Mode_Selection =LSM303DLHC_HPM_NORMAL_MODE;
  LSM303DLHCFilter_InitStructure.HighPassFilter_CutOff_Frequency = LSM303DLHC_HPFCF_16;
  LSM303DLHCFilter_InitStructure.HighPassFilter_AOI1 = LSM303DLHC_HPF_AOI1_DISABLE;
  LSM303DLHCFilter_InitStructure.HighPassFilter_AOI2 = LSM303DLHC_HPF_AOI2_DISABLE;

  /* Configure the accelerometer LPF main parameters */
  LSM303DLHC_AccFilterConfig(&LSM303DLHCFilter_InitStructure);
}

/**
* @brief Read LSM303DLHC output register, and calculate the acceleration ACC=(1/SENSITIVITY)* (out_h*256+out_l)/16 (12 bit rappresentation)
* @param pnData: pointer to float buffer where to store data
* @retval None
*/
void Acc_ReadData(float* pfData)
{
  int16_t pnRawData[3];
  uint8_t ctrlx[2];
  float LSM_Acc_Sensitivity = LSM_Acc_Sensitivity_2g;
  uint8_t buffer[6], cDivider;
  uint8_t i = 0;
  
  /* Read the register content */
  LSM303DLHC_Read(ACC_I2C_ADDRESS, LSM303DLHC_CTRL_REG4_A, ctrlx,2);
  LSM303DLHC_Read(ACC_I2C_ADDRESS, LSM303DLHC_OUT_X_L_A, buffer, 6);
 
  
  if(ctrlx[1]&0x40)
    cDivider=64;
  else
    cDivider=16;

  /* check in the control register4 the data alignment*/
  if(!(ctrlx[0] & 0x40) || (ctrlx[1] & 0x40)) /* Little Endian Mode or FIFO mode */
  {
    for(i=0; i<3; i++)
    {
      pnRawData[i]=((int16_t)((uint16_t)buffer[2*i+1] << 8) + buffer[2*i])/cDivider;
    }
  }
  else /* Big Endian Mode */
  {
    for(i=0; i<3; i++)
      pnRawData[i]=((int16_t)((uint16_t)buffer[2*i] << 8) + buffer[2*i+1])/cDivider;
  }
  /* Read the register content */
  LSM303DLHC_Read(ACC_I2C_ADDRESS, LSM303DLHC_CTRL_REG4_A, ctrlx,2);


  if(ctrlx[1]&0x40)
  {
    /* FIFO mode */
    LSM_Acc_Sensitivity = 0.25;
  }
  else
  {
    /* normal mode */
    /* switch the sensitivity value set in the CRTL4*/
    switch(ctrlx[0] & 0x30)
    {
    case LSM303DLHC_FULLSCALE_2G:
      LSM_Acc_Sensitivity = LSM_Acc_Sensitivity_2g;
      break;
    case LSM303DLHC_FULLSCALE_4G:
      LSM_Acc_Sensitivity = LSM_Acc_Sensitivity_4g;
      break;
    case LSM303DLHC_FULLSCALE_8G:
      LSM_Acc_Sensitivity = LSM_Acc_Sensitivity_8g;
      break;
    case LSM303DLHC_FULLSCALE_16G:
      LSM_Acc_Sensitivity = LSM_Acc_Sensitivity_16g;
      break;
    }
  }

  /* Obtain the mg value for the three axis */
  for(i=0; i<3; i++)
  {
    pfData[i]=(float)pnRawData[i]/LSM_Acc_Sensitivity;
  }
}

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
