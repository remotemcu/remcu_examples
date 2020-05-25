/**
  @page ADC_ExtLinesTrigger ADC external lines trigger example
  
  @verbatim
  ******************** (C) COPYRIGHT 2011 STMicroelectronics *******************
  * @file    ADC/ExtLinesTrigger/readme.txt 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Description of the ADC external lines trigger example.
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

This example describes how to trigger ADC regular and injected groups channels 
conversion using two external line events. Discontinuous mode is enabled for regular
group channel conversion and configured to convert one regular channel on each 
external trigger.

ADC1 is configured to start regular group channel conversion on EXTI11 event.
On detection of the first rising edge on PE.11 pin (PF.11 pin for High-Density Value line),
the conversion of the first regular channel (ADC channel4) is done and its converted 
value is transferred by DMA to ADC_RegularConvertedValueTab table. On the following edge 
detection, the second regular channel (ADC channel14) is automatically converted and 
its converted value is stored by DMA in the same table. The number of transmitted data 
by DMA, in this example is limited to 64 data.

The procedure is repeated for both regular channels on each EXTI11 event.
ADC1 is configured to start injected group channel conversion on EXTI15 event.
On detection of the first rising edge on PE.15 pin all selected injected channels, which
are two in this example (ADC channel11 and channel12), are converted and an interrupt 
is generated on JEOC flag rising at the end of all injected channels conversion. 
Both injected channels converted results are stored in ADC_InjectedConvertedValueTab 
table inside the interrupt routine.
The procedure is repeated for injected channels on each EXTI15 event.
The ADC1 clock is set to 12 MHz on Value line devices and to 14MHz on other 
devices.

@par Directory contents 

  - ADC/ExtLinesTrigger/stm32f10x_conf.h     Library Configuration file
  - ADC/ExtLinesTrigger/stm32f10x_it.c       Interrupt handlers
  - ADC/ExtLinesTrigger/stm32f10x_it.h       Interrupt handlers header file
  - ADC/ExtLinesTrigger/system_stm32f10x.c   STM32F10x system source file  
  - ADC/ExtLinesTrigger/main.c               Main program

@par Hardware and Software environment 

  - This example runs on STM32F10x Connectivity line, High-Density, Medium-Density, 
    XL-Density, Medium-Density Value line, Low-Density and Low-Density Value line Devices.
  
  - This example has been tested with STMicroelectronics STM32100B-EVAL (Medium-Density
    Value line), STM3210C-EVAL (Connectivity line), STM3210E-EVAL (High-Density and
    XL-Density) and STM3210B-EVAL (Medium-Density) evaluation boards and can be easily
    tailored to any other supported device and development board.

  - STM32100B-EVAL Set-up
    - Connect a known voltage, between 0-3.3V, to ADC Channel14 mapped on pin 
      PC.04 (potentiometer RV2), ADC Channel4 mapped on pin PA.04, ADC Channel11 
      mapped on pin PC.01 and ADC Channel12 mapped on pin PC.02.
    - Connect a push-button to pin PE.11 (EXTI Line11) and another push-button 
      to pin PE.15 (EXTI Line15).
    @note Make shure that jumper JP3 is open.
    
  - STM32100E-EVAL Set-up 
    - Connect a known voltage, between 0-3.3V, to ADC Channel14 mapped on pin 
      PC.04 (potentiometer RV1), ADC Channel4 mapped on pin PA.04, ADC Channel11 
      mapped on pin PC.01 and ADC Channel12 mapped on pin PC.02.
    - Connect a push-button to pin PF.11 (EXTI Line11) and another push-button 
      to pin PE.15 (EXTI Line15).

  - STM3210C-EVAL Set-up 
    - Connect a known voltage, between 0-3.3V, to ADC Channel14 mapped on pin 
      PC.04 (potentiometer RV1), ADC Channel4 mapped on pin PA.04, ADC Channel11 
      mapped on pin PC.01 and ADC Channel12 mapped on pin PC.02.
    - Connect a push-button to pin PE.11 (EXTI Line11) and another push-button 
      to pin PE.15 (EXTI Line15).

  - STM3210E-EVAL Set-up 
    - Connect a known voltage, between 0-3.3V, to ADC Channel14 mapped on pin 
      PC.04 (potentiometer RV1), ADC Channel4 mapped on pin PA.04, ADC Channel11 
      mapped on pin PC.01 and ADC Channel12 mapped on pin PC.02.
    - Connect a push-button to pin PE.11 (EXTI Line11) and another push-button 
      to pin PE.15 (EXTI Line15).

  - STM3210B-EVAL Set-up 
    - Connect a known voltage, between 0-3.3V, to ADC Channel14 mapped on pin 
      PC.04 (potentiometer RV1), ADC Channel4 mapped on pin PA.04, ADC Channel11 
      mapped on pin PC.01 and ADC Channel12 mapped on pin PC.02.
    - Connect a push-button to pin PE.11 (EXTI Line11) and another push-button 
      to pin PE.15 (EXTI Line15).              
   
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
