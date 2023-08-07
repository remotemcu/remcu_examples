/** @page TIM1_DMA  Use DMA1 with TIM1 Update request to transfer Data from memory to TIM1 CCR1 register (Capture Compare Register 1  

  @verbatim
  ******************** (C) COPYRIGHT 2013 STMicroelectronics *******************
  * @file    TIM1/TIM1_DMA/readme.txt 
  * @author  MCD Application Team
  * @version V1.5.2
  * @date    30-September-2014
  * @brief   Description of the TIM1 DMA Example.
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

  This example provides a description of how to use DMA1 with TIM1 Update 
  request to transfer Data from memory to TIM1 Capture Compare Register 1.
  TIM1CLK is set to 16 MHz, the TIM1 Prescaler is equal to 1 and TIM1 Auto 
  Reload register is equal to 4095. So The TIM1 Channel 1 is configured to 
  generate PWM signal with a frequency equal to 3.9 KHz and a variable duty 
  cycle (defined by Capture Compare Register 1 content   that is updated by the
  DMA after a specific number of Update event.
  The number of this repetitive requests is defined by the TIM1 Repetition 
  counter, each 3 Update Requests, the TIM1 Channel 1 Duty Cycle changes to the 
  next new value defined by SrcBuffer.


  @par Directory contents

  - TIM1/TIM1_DMA/main.c              Main file containing the "main" function
  - TIM1/TIM1_DMA/stm8l15x_conf.h     Library Configuration file
  - TIM1/TIM1_DMA/stm8l15x_it.c       Interrupt routines source
  - TIM1/TIM1_DMA/stm8l15x_it.h       Interrupt routines declaration


  @par Hardware and Software environment

    - This example runs on STM8L15x High-Density, Medium-Density Plus and Medium-Density
     Devices.
  
  - This example has been tested with STMicroelectronics STM8L1528-EVAL 
    (STM8L15x High-Density devices) and STM8L1526-EVAL (STM8L15x Medium-Density 
    devices) evaluation boards and can be easily tailored to any other
    development board.

  - STM8L1528-EVAL and STM8L1526-EVAL Set-up
    - Make sure that the LCD glass daughter board is mounted in IO position.
      For more details please refer to the evaluation board user manual.
    - Connect TIM1 Channel 1 pin (PD2) to an oscilloscope
  
  @par How to use it ?

  In order to make the program work, you must do the following :

  - Copy all source files from this example folder to the template folder under
    Project\STM8L15x_StdPeriph_Template
  - Open your preferred toolchain 
  - Rebuild all files and load your image into target memory
  - Run the example
   - Connect TIM1 Channel 1 (PD2) to an oscilloscope and note that the duty cycle 
     is updated each 3 times (50%, 25%, 12.5%, 50%, 25% .... ) 




 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
