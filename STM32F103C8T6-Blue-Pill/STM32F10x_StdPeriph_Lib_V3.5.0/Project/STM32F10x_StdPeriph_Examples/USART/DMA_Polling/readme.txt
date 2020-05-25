/**
  @page USART_DMA_Polling USART DMA Polling example
  
  @verbatim
  ******************** (C) COPYRIGHT 2011 STMicroelectronics *******************
  * @file    USART/DMA_Polling/readme.txt 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Description of the USART DMA Polling example.
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

This example provides a basic communication between USARTy and USARTz using DMA 
capability. USARTy and USARTz can be USART1 and USART2 or USART2 and USART3,
depending on the STMicroelectronics EVAL board you are using.

First, the DMA transfers data from TxBuffer2 buffer to USARTz Transmit data 
register, then this data is sent to USARTy. Data received by USARTy is transferred
by DMA and stored in RxBuffer1 then compared with the send ones and the result 
of this comparison is stored in the "TransferStatus1" variable.
 
In the same time, the DMA transfers data from TxBuffer1 buffer to USARTy Transmit
data register, then this data is sent to USARTz. Data received by USARTz is
transferred by DMA and stored in RxBuffer2 then compared with the send ones and
the result of this comparison is stored in the "TransferStatus2" variable. 

USARTy and USARTz configured as follow:
  - BaudRate = 230400 baud  
  - Word Length = 8 Bits
  - One Stop Bit
  - No parity
  - Hardware flow control disabled (RTS and CTS signals)
  - Receive and transmit enabled       
        
@par Directory contents 

  - USART/DMA_Polling/platform_config.h    Evaluation board specific configuration file
  - USART/DMA_Polling/stm32f10x_conf.h     Library Configuration file
  - USART/DMA_Polling/stm32f10x_it.h       Interrupt handlers header file
  - USART/DMA_Polling/stm32f10x_it.c       Interrupt handlers
  - USART/DMA_Polling/main.c               Main program
  - USART/DMA_Polling/system_stm32f10x.c   STM32F10x system source file
  
@par Hardware and Software environment 

  - This example runs on STM32F10x Connectivity line, High-Density, High-Density 
    Value line, Medium-Density, XL-Density, Medium-Density Value line, Low-Density 
    and Low-Density Value line Devices.
  
  - This example has been tested with STMicroelectronics STM32100E-EVAL (High-Density
    Value line), STM32100B-EVAL (Medium-Density Value line), STM3210C-EVAL (Connectivity line), 
    STM3210E-EVAL (High-Density and XL-Density) and STM3210B-EVAL (Medium-Density) 
    evaluation boards and can be easily tailored to any other supported device 
    and development board.
    To select the STMicroelectronics evaluation board used to run the example, 
    uncomment the corresponding line in USART/DMA_Polling/platform_config.h file

  - STM32100E-EVAL Set-up 
    - Connect a null-modem female/female RS232 cable between CN10 (USART1) and 
      CN5 (USART2).
      @note Make sure that jumper JP5 is not open.
      
  - STM32100B-EVAL Set-up
    - Connect a null-modem female/female RS232 cable between CN10 (USART1) and 
      CN9 (USART2).
      @note In this case USART2 Tx and Rx pins are remapped by software on PD.05 
            and PD.06 respectively.

  - STM3210C-EVAL Set-up 
    - Connect USART2 Tx pin (PD.05) to USART3 Rx pin (PC.11)
    - Connect USART2 Rx pin (PD.06) to USART3 Tx pin	(PC.10)
      @note In this case USART3 Tx and Rx pins are remapped by software.
            Make sure that jumpers JP19 and JP18 are open.
              
  - STM3210E-EVAL Set-up 
    - Connect a null-modem female/female RS232 cable between CN12 (USART1) and 
      CN8 (USART2).
    
  - STM3210B-EVAL Set-up
    - Connect a null-modem female/female RS232 cable between CN6 (USART1) and 
      CN5 (USART2).
      - In this case USART2 Tx and Rx pins are remapped by software on PD.05 
        and PD.06 respectively. 
  
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
