/** @page DAC_SignalsGeneration Use DAC with DMA to generate square, triangular, sinusoidal and sincard signals

  @verbatim
  ******************** (C) COPYRIGHT 2013 STMicroelectronics *******************
  * @file    DAC/DAC_SignalsGeneration/readme.txt 
  * @author  MCD Application Team
  * @version V1.5.2
  * @date    30-September-2014
  * @brief   Description of the DAC signals generation Example.
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

  This example provides a short description of how to use the DAC peripheral
  to generate several signal using DMA memory to peripheral mode.
  By Default, DMA transfers continuously to the DAC, the Triangular Table waveform.
  When the user presses the LEFT button, DMA transfers the selected waveform to DAC.
  Four waveforms are available: Triangular waveform, Square waveform, Sinc waveform
  and Sine waveform.
  When the user press the RIGHT button, DMA transfer frequency is increased, the
  frequency of the selected waveform is multiplied by 2.
  Five different frequencies are available.
  

  @par Directory contents

  - DAC/DAC_SignalsGeneration/main.c              Main file
  - DAC/DAC_SignalsGeneration/main.h              Main file declaration
  - DAC/DAC_SignalsGeneration/stm8l15x_conf.h     Library Configuration file
  - DAC/DAC_SignalsGeneration/stm8l15x_it.c       Interrupt routines source
  - DAC/DAC_SignalsGeneration/stm8l15x_it.h       Interrupt routines declaration


  @par Hardware and Software environment

    - This example runs on STM8L15x High-Density, Medium-Density Plus and Medium-Density 
    Devices.
  
  - This example has been tested with STMicroelectronics STM8L1528-EVAL 
    (STM8L15x High-Density devices) and STM8L1526-EVAL (STM8L15x Medium-Density
    devices) evaluation boards and can be easily tailored to any other
    development board.

  - STM8L1526-EVAL Set-up
     - Make sure that the LCD glass daughter board is mounted in IO position.
     - LEFT and RIGHT Joystick push-buttons.
     - BNC connector CN15 (DAC_OUT)  
     - Make sure that jumper JP10 is in BNC position.
  
  - STM8L1528-EVAL Set-up
     - Make sure that the LCD glass daughter board is mounted in IO position.
       For more details please refer to the evaluation board user manual.
     - LEFT and RIGHT Joystick push-buttons.
     - BNC connector CN2 (DAC_OUT)  
     - Make sure that jumper JP3 is removed.
  
  @par How to use it ?

  In order to make the program work, you must do the following :

  - Copy all source files from this example folder to the template folder under
    Project\STM8L15x_StdPeriph_Template
  - Open your preferred toolchain 
  - Rebuild all files and load your image into target memory
  - Add the required file:
    - stm8_eval.c   (under Utilities\STM8_EVAL)
  - Run the example
  - Connect an oscilloscope to BNC connector. 
  - Press Joystick LEFT Button to change the waveform.
  - Press Joystick RIGHT Button to change the waveform frequency.



 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
