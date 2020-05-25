/**
  @page ADC_3ADCs_DMA 3 ADCs DMA example
  
  @verbatim
  ******************** (C) COPYRIGHT 2011 STMicroelectronics *******************
  * @file    ADC/3ADCs_DMA/readme.txt 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Description of the 3ADCs DMA example.
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

This example describes how to use the 3 ADCs in independent conversions. 
Two of them; ADC1 and ADC3 are transferring continuously converted data using DMA
while ADC2 converted data are stored using End of conversion interrupt.

ADC1 is configured to convert continuously ADC channel14. Each time an end of
conversion occurs the DMA1 transfers, in circular mode, the converted data from 
ADC1 DR register to the ADC1_ConvertedValue variable.

ADC2 is configured to convert continuously ADC channel13. Each time an end of
conversion occurs an end of conversion interrupt is generated and inside the
interrupt routine the converted data are read from ADC2 DR register and stored
into the ADC2_ConvertedValue variable.

ADC3 is configured to convert continuously ADC channel12. Each time an end of
conversion occurs the DMA2 transfers, in circular mode, the converted data from
ADC3 DR register to the ADC3_ConvertedValue variable.

The ADCs clocks are set to 14 MHz.

The result of ADC1, ADC2 and ADC3 conversion is monitored through the three
variables: ADC1ConvertedValue, ADC2ConvertedValue and ADC3ConvertedValue.

@par Directory contents 

  - ADC/3ADCs_DMA/stm32f10x_conf.h    Library Configuration file
  - ADC/3ADCs_DMA/stm32f10x_it.c      Interrupt handlers
  - ADC/3ADCs_DMA/stm32f10x_it.h      Interrupt handlers header file
  - ADC/3ADCs_DMA/system_stm32f10x.c  STM32F10x system source file
  - ADC/3ADCs_DMA/main.c              Main program

@par Hardware and Software environment 

  - This example runs only on STM32F10x High-Density and XL-Density Devices.
  
  - This example has been tested with STMicroelectronics STM3210E-EVAL (High-Density
    and XL-Density) evaluation board and can be easily tailored to any other 
    supported device and development board.

  - STM3210E-EVAL Set-up 
    - Connect a variable power supply 0-3.3V to ADC3 Channel12 mapped on pin PC.02
    - Connect a variable power supply 0-3.3V to ADC2 Channel13 mapped on pin PC.03
    - Connect a variable power supply 0-3.3V to ADC1 Channel14 mapped on pin PC.04
      (potentiometer RV1 on STM3210E-EVAL board)
      
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
