/**
  @page ADC_TIMTrigger_AutoInjection ADC TIM trigger and auto-injection example
  
  @verbatim
  ******************** (C) COPYRIGHT 2011 STMicroelectronics *******************
  * @file    ADC/TIMTrigger_AutoInjection/readme.txt 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Description of the ADC TIM trigger and auto-injection example.
  ******************************************************************************
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  ******************************************************************************
   @endverbatim

@par Example Description 

This example describes how to convert ADC regular group channels continuously using
TIM1 external trigger and injected group channels using the auto-injected feature.

ADC1 is configured to convert on each TIM1 capture compare event ADC channel14.
Each time an end of regular conversion occurs the DMA transfers the converted data 
from ADC1 DR register to the ADC_RegularConvertedValueTab table. 
Enabling the auto-injected feature, allows to convert automatically the injected 
channel ADC channel11 after the end of the regular channel14. An ADC interrupt is 
generated then by JEOC flag at the end of the injected channel conversion and in 
the interrupt routine the result is stored in the ADC_InjectedConvertedValueTab table.
The procedure is repeated 32 times then the TIM1 peripheral is disabled and thus, 
no conversion will be done neither regular nor injected.
TIM1 start conversion triggers can be visualized on oscilloscope on PA.08 and at the
same time the toggle of pin PC.06 which indicates the automatic auto-injection conversion.

The ADC1 clock is set to 12 MHz on Value line devices and to 14MHz on other 
devices.

@par Directory contents 

  - ADC/TIMTrigger_AutoInjection/stm32f10x_conf.h     Library Configuration file
  - ADC/TIMTrigger_AutoInjection/stm32f10x_it.c       Interrupt handlers
  - ADC/TIMTrigger_AutoInjection/stm32f10x_it.h       Interrupt handlers header file
  - ADC/TIMTrigger_AutoInjection/system_stm32f10x.c   STM32F10x system source file  
  - ADC/TIMTrigger_AutoInjection/main.c               Main program

@par Hardware and Software environment 

  - This example runs on STM32F10x Connectivity line, High-Density, High-Density Value line,
    Medium-Density, XL-Density, Medium-Density Value line, Low-Density and Low-Density Value 
    line Devices.
  
  - This example has been tested with STMicroelectronics STM32100E-EVAL(High-Density 
    Value line) STM32100B-EVAL (Medium-Density Value line), STM3210C-EVAL (Connectivity
    line), STM3210E-EVAL (High-Density and XL-Density) and STM3210B-EVAL (Medium-Density) 
    evaluation boards and can be easily tailored to any other supported device and development 
    board.

  - STM32100E-EVAL Set-up 
    - Connect a known voltage, between 0-3.3V, to ADC Channel14 mapped on pin 
      PC.04 (potentiometer RV1) and ADC Channel11 mapped on pin PC.01
    - Connect PA.08 and PC.06 pins to an oscilloscope
    
  - STM32100B-EVAL Set-up
    - Connect a variable power supply 0-3.3V to ADC Channel14 mapped on pin PC.04
      (potentiometer RV2) and ADC Channel11 mapped on pin PC.01
    - Connect PA.08 and PC.06 pins to an oscilloscope

  - STM3210C-EVAL Set-up
    - Connect a known voltage, between 0-3.3V, to ADC Channel14 mapped on pin 
      PC.04 (potentiometer RV1) and ADC Channel11 mapped on pin PC.01
    - Connect PA.08 and PC.06 pins to an oscilloscope
    
  - STM3210E-EVAL Set-up 
    - Connect a known voltage, between 0-3.3V, to ADC Channel14 mapped on pin 
      PC.04 (potentiometer RV1) and ADC Channel11 mapped on pin PC.01
    - Connect PA.08 and PC.06 pins to an oscilloscope

  - STM3210B-EVAL Set-up 
    - Connect a known voltage, between 0-3.3V, to ADC Channel14 mapped on pin 
      PC.04 (potentiometer RV1) and ADC Channel11 mapped on pin PC.01
    - Connect PA.08 and PC.06 pins to an oscilloscope        
      
@par How to use it ? 

In order to make the program work, you must do the following :
 - Copy all source files from this example folder to the template folder under
   Project\STM32F10x_StdPeriph_Template
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example 

@note
 - Low-density Value line devices are STM32F100xx microcontrollers where the 
   Flash memory density ranges between 16 and 32 Kbytes.
 - Low-density devices are STM32F101xx, STM32F102xx and STM32F103xx 
   microcontrollers where the Flash memory density ranges between 16 and 32 Kbytes.
 - Medium-density Value line devices are STM32F100xx microcontrollers where
   the Flash memory density ranges between 64 and 128 Kbytes.  
 - Medium-density devices are STM32F101xx, STM32F102xx and STM32F103xx 
   microcontrollers where the Flash memory density ranges between 64 and 128 Kbytes.
 - High-density Value line devices are STM32F100xx microcontrollers where
   the Flash memory density ranges between 256 and 512 Kbytes.  
 - High-density devices are STM32F101xx and STM32F103xx microcontrollers where
   the Flash memory density ranges between 256 and 512 Kbytes.
 - XL-density devices are STM32F101xx and STM32F103xx microcontrollers where
   the Flash memory density ranges between 512 and 1024 Kbytes.
 - Connectivity line devices are STM32F105xx and STM32F107xx microcontrollers.
 
 * <h3><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h3>
 */
