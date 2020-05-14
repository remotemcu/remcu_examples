/**
  ******************************************************************************
  * @file    platform_config.h 
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    20-September-2012
  * @brief   Evaluation board specific configuration file.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PLATFORM_CONFIG_H
#define __PLATFORM_CONFIG_H

/* Includes ------------------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

#define USB_INT_DEFAULT /* For Default Interrupt Mode */
//#define USB_INT_REMAP /* For Remapping Interrupt Mode */

#if defined (USB_INT_REMAP)
#warning "You are running Joystick Demo using Remapping Interruption Mode"
#endif

#if !defined (USB_INT_DEFAULT) && !defined (USB_INT_REMAP)
  #error "Missing define Please Define Your Interrupt Mode By UnComment Line in platform_config.h File"
#endif


  #define USB_DISCONNECT                      GPIOE  
  #define USB_DISCONNECT_PIN                  GPIO_Pin_14
  #define RCC_AHBPeriph_GPIO_DISCONNECT       RCC_AHBPeriph_GPIOE
                                     
#define RCC_AHBPeriph_ALLGPIO                 (RCC_AHBPeriph_GPIOA \
                                              | RCC_AHBPeriph_GPIOB \
                                              | RCC_AHBPeriph_GPIOC \
                                              | RCC_AHBPeriph_GPIOD \
                                              | RCC_AHBPeriph_GPIOE \
                                              | RCC_AHBPeriph_GPIOF )



/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __PLATFORM_CONFIG_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

