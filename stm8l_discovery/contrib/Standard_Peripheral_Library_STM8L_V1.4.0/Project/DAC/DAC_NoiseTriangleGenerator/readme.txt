/** @page DAC_NoiseTriangleGenerator Use DAC Noise and Triangle Generator feature

  @verbatim
  ******************** (C) COPYRIGHT 2013 STMicroelectronics *******************
  * @file    DAC/DAC_NoiseTriangleGenerator/readme.txt 
  * @author  MCD Application Team
  * @version V1.5.2
  * @date    30-September-2014
  * @brief   Description of the DAC Noise and Triangle Generation Example.
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
  to generate, automatically, Noise and Triangle waveform.
  By Default, DAC channel1 is generating a Triangle waveform   and DAC channel2
  is generating a white Noise waveform with the maximum amplitude.
  When the user presses the LEFT button,the Triangle waveform Amplitude is decreasing.
  When the user presses the RIGHT button,the Noise waveform Amplitude is decreasing.
  

  @par Directory contents

  - DAC/DAC_Noise&TriangleGenerator/main.c            Main file
  - DAC/DAC_Noise&TriangleGenerator/stm8l15x_conf.h   Library Configuration file
  - DAC/DAC_Noise&TriangleGenerator/stm8l15x_it.c     Interrupt routines source
  - DAC/DAC_Noise&TriangleGenerator/stm8l15x_it.h     Interrupt routines declaration

  @par Hardware and Software environment

  - This example runs on STM8L15x High-Density devices.
  
  - This example has been tested with STMicroelectronics STM8L1528-EVAL
    evaluation boards and can be easily tailored to any other development board.
    
  - STM8L1528-EVAL Set-up
     - Make sure that the LCD glass daughter board is mounted in IO position.
       For more details please refer to the evaluation board user manual.
     - LEFT and RIGHT Joystick push-buttons.
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
  - Connect an oscilloscope to DAC_Channel1 (PF.0) and DAC_Channel_2 (PF.1).
  - Press Joystick LEFT Button to change the DAC channel 1 Triangle waveform amplitude.
  - Press Joystick RIGHT Button to change the  DAC channel 2 Noise waveform amplitude.
  


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
