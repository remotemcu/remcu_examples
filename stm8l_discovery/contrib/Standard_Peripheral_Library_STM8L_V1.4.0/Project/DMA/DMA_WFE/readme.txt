/** @page DMA_WFE Use DMA transfer event to wake up CPU from WFE low power mode. 

  @verbatim
  ******************** (C) COPYRIGHT 2013 STMicroelectronics *******************
  * @file    DMA/DMA_WFE/readme.txt 
  * @author  MCD Application Team
  * @version V1.5.2
  * @date    30-September-2014
  * @brief   Description of the DMA WFE example.
  ******************************************************************************
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
   @endverbatim
   
  @par Example description

  This example provides a short description of how to use the DMA peripheral in
  circular mode to transfer data from USART peripheral to RAM memory. 
  When the user sends a character using USART, this character will be 
  transferred using DMA to a specific RAM buffer and a DMA event is provided by
  USART_RX.
  After configuring the DMA channel to transfer data (4 bytes   from USART data
  register to a RAM buffer, the MCU enters wfe mode.
  While waiting the “DMA Transfer complete” event, the CPU is in wfe mode. 
  As soon as the “DMA Transfer complete” event occurs, the CPU wakes up from wfe 
  mode and executes a specific action (activate/deactivate Leds   depending 
  on the Data in the RAM buffer and returns to the wfe mode (by software  .
  
  Since the DMA channel is configured in circular mode, the same procedure 
  will be repeated definitely.
  Writing "led1" on HyperTerminal, will turn on LED1 and all the other leds will 
  be turned off.
  Writing "led2" on HyperTerminal, will turn on LED2 and all the other leds will 
  be turned off.
  Writing "led3" on HyperTerminal, will turn on LED3 and all the other leds will 
  be turned off.
  Writing "led4" on HyperTerminal, will turn on LED4 and all the other leds will 
  be turned off.
  Writing anything else than "led1", "led2", "led3" or "led4", will turn on all 
  leds.
  
  @par Directory contents

  - DMA/DMA_WFE/main.c            Main file containing the "main" function
  - DMA/DMA_WFE/stm8l15x_conf.h   Library Configuration file
  - DMA/DMA_WFE/stm8l15x_it.c     Interrupt routines source
  - DMA/DMA_WFE/stm8l15x_it.h     Interrupt routines declaration



 @par Hardware and Software environment

    - This example runs on STM8L15x High-Density, Medium-Density Plus, Medium-Density
    and Low-Density Devices.
  
  - This example has been tested with STMicroelectronics STM8L1528-EVAL 
    (STM8L15x High-Density devices) and STM8L1526-EVAL (STM8L15x Medium-Density and
    Low-Density devices) evaluation boards and can be easily tailored to any other
    development board.   

  - STM8L1526-EVAL Set-up
     - LED1, LED2,LED3 and LED4.
     - Connect a null-modem female/female RS232 cable between the DB9 connector 
       CN1 and PC serial port.
     - Make sure that the LCD glass daughter board is mounted in IO position.
       For more details please refer to the evaluation board user manual.  
     - Make sure that JP5 jumper is in RS232 position

  
  - STM8L1528-EVAL Set-up
     - Make sure that the LCD glass daughter board is mounted in IO position.
       For more details please refer to the evaluation board user manual.
     - LED1, LED2,LED3 and LED4.
     - Connect a null-modem female/female RS232 cable between the DB9 connector 
       CN7 and PC serial port.
     - Make sure that JP5, JP11 and JP12 jumpers are in RS232 position

  
  - HyperTerminal configuration:
     - Word Length = 8 Bits
     - One Stop Bit
     - No parity
     - BaudRate = 9600 baud
     - flow control: None 
      

  @par How to use it ?

  In order to make the program work, you must do the following :

  - Copy all source files from this example folder to the template folder under
    Project\STM8L15x_StdPeriph_Template
  - Open your preferred toolchain 
  - Add the required file:
    - stm8_eval.c   (under Utilities\STM8_EVAL)
  - Rebuild all files and load your image into target memory
  - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */