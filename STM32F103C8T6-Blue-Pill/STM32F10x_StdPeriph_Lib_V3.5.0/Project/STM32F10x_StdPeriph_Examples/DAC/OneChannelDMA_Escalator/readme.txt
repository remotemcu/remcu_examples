/**
  @page DAC_OneChannelDMA_Escalator DAC one channel DMA escalator example
  
  @verbatim
  ******************** (C) COPYRIGHT 2011 STMicroelectronics *******************
  * @file    DAC/OneChannelDMA_Escalator/readme.txt 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Description of the DAC one channel DMA escalator example.
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

This example describes how to use one DAC channel mode with DMA to generate an
escalator signal on DAC channel1 output.

DAC channel1 conversion is configured to be triggered by TIM6 TRGO triggers and
without noise/triangle wave generation. 8bit right data alignment is selected
since we choose to access DAC_DHR8R1 register.
DMA2 channel3 is configured to transfer continuously, byte by byte, a 6-byte
buffer to the DAC1 register DAC_DHR8R1.

The transferred 6bytes buffer is made to have an escalator signal on DAC channel1
output. DAC channel1 is then enabled. Once TIM6 is enabled, each TIM6 TRGO update
event generate a DMA request which transfer data  to the DAC1 register and DAC
conversion is started. The escalator signal can be visualized by connecting PA.04
pin to an oscilloscope.

@par Directory contents 

  - DAC/OneChannelDMA_Escalator/stm32f10x_conf.h     Library Configuration file
  - DAC/OneChannelDMA_Escalator/stm32f10x_it.c       Interrupt handlers
  - DAC/OneChannelDMA_Escalator/stm32f10x_it.h       Header for stm32f10x_it.c
  - DAC/OneChannelDMA_Escalator/main.c               Main program
  - DAC/OneChannelDMA_Escalator/system_stm32f10x.c   STM32F10x system source file
  
@par Hardware and Software environment 

  - This example runs on STM32F10x Connectivity line, High-Density, XL-Density,
    High-Density Value line, Medium-Density Value line and Low-Density Value 
    line Devices.
  
  - This example has been tested with STMicroelectronics STM32100B-EVAL 
    (Medium-Density Value line),STM32100E-EVAL (High-Density Value line),
     STM3210C-EVAL (Connectivity line) and STM3210E-EVAL (High-Density and 
     XL-Density) evaluation boards and can be easily tailored to any other 
     supported device and development board.   

  - STM32100B-EVAL Set-up 
    - Connect PA.04 pin to an oscilloscope 
    @note Make shure that jumper JP2 is open.
    
  - STM3210C-EVAL Set-up 
    - Connect PA.04 pin to an oscilloscope
    @note Make shure that jumper JP15 is open.
        
  - STM3210E-EVAL Set-up 
    - Connect PA.04 pin to an oscilloscope
	
  - STM32100E-EVAL Set-up 
    - Connect PA.04 pin to an oscilloscope
    
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
