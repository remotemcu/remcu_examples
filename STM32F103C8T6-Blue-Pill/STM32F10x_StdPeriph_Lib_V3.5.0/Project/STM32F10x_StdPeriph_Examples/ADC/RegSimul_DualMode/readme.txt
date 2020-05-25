/**
  @page ADC_RegSimul_DualMode ADC regular simultaneous dual mode example
  
  @verbatim
  ******************** (C) COPYRIGHT 2011 STMicroelectronics *******************
  * @file    ADC/RegSimul_DualMode/readme.txt 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Description of the ADC regular simultaneous dual mode example.
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

This example describes how to use ADC1 and ADC2 in regular simultaneous dual mode.
ADC1 is configured to convert channel14 and channel17 regular channels continuously.
ADC2 is configured to convert channel11 and channel12 regular channels continuously.
The connection between internal Vref and channel17 is enabled for ADC1.

Once the regular channels conversion is started by software, channel14 is converted
on ADC1 and channel11 is converted on ADC2 on the same time. The 32bits conversion 
result is then stored on ADC1 DR register. The DMA will transfer this data which
will be stored ADC_DualConvertedValueTab table. Consecutively to those conversion,
channel17 is converted on ADC1 and channel12 on ADC2. The combined conversion
result is also transferred by DMA to the same destination buffer. 

The same procedure is repeated until the specified number of data to be transferred
by DMA is reached.

The ADCs clocks are set to 14 MHz.

@par Directory contents 

  - ADC/RegSimul_DualMode/stm32f10x_conf.h     Library Configuration file
  - ADC/RegSimul_DualMode/stm32f10x_it.c       Interrupt handlers
  - ADC/RegSimul_DualMode/stm32f10x_it.h       Interrupt handlers header file
  - ADC/RegSimul_DualMode/system_stm32f10x.c   STM32F10x system source file
  - ADC/RegSimul_DualMode/main.c               Main program

@par Hardware and Software environment 

  - This example runs on STM32F10x Connectivity line, High-Density, Medium-Density, 
    XL-Density and Low-Density Devices.

  - This example has been tested with STMicroelectronics STM3210C-EVAL 
    (Connectivity line), STM3210E-EVAL (High-Density and XL-Density) and
    STM3210B-EVAL (Medium-Density) evaluation boards and can be easily tailored
    to any other supported device and development board.

  - STM3210C-EVAL Set-up 
    - Connect a known voltage, between 0-3.3V, to ADC Channel14 mapped on pin 
      PC.04 (potentiometer RV1), ADC Channel11 mapped on pin PC.01 and ADC 
      Channel12 mapped on pin PC.02.

  - STM3210E-EVAL Set-up 
    - Connect a known voltage, between 0-3.3V, to ADC Channel14 mapped on pin 
      PC.04 (potentiometer RV1), ADC Channel11 mapped on pin PC.01 and ADC 
      Channel12 mapped on pin PC.02.

  - STM3210B-EVAL Set-up 
    - Connect a known voltage, between 0-3.3V, to ADC Channel14 mapped on pin 
      PC.04 (potentiometer RV1), ADC Channel11 mapped on pin PC.01 and ADC 
      Channel12 mapped on pin PC.02.          

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
 - High-density devices are STM32F101xx and STM32F103xx microcontrollers where
   the Flash memory density ranges between 256 and 512 Kbytes.
 - XL-density devices are STM32F101xx and STM32F103xx microcontrollers where
   the Flash memory density ranges between 512 and 1024 Kbytes.
 - Connectivity line devices are STM32F105xx and STM32F107xx microcontrollers.
    
 * <h3><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h3>
 */
