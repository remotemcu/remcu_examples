/** @page COMP1_Halt  Use Comparator 1 to exit MCU from Halt mode

  @verbatim
  ******************** (C) COPYRIGHT 2013 STMicroelectronics *******************
  * @file    COMP/COMP1_Halt/readme.txt 
  * @author  MCD Application Team
  * @version V1.5.2
  * @date    30-September-2014
  * @brief   Description of the COMP1 Halt Example.
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

  This example shows how to configure the COMP peripheral to exit MCU from Halt 
  mode.
  When the user presses the key push-button, the MCU enters Halt mode. 
  And when the comparator 1 non inverting input (which is connected to the
  potentiometer) exceeds the internal reference voltage which is set to 1.22V, 
  the comparator 1 causes the CPU to exit halt mode.


  @par Directory contents

  - COMP\COMP1_Halt\main.c             Main file containing the "main" function
  - COMP\COMP1_Halt\stm8l15x_conf.h    Library Configuration file
  - COMP\COMP1_Halt\stm8l15x_it.c      Interrupt routines source
  - COMP\COMP1_Halt\stm8l15x_it.h      Interrupt routines declaration


  @par Hardware and Software environment

    - This example runs on STM8L15x High-Density, Medium-Density Plus, Medium-Density
    and Low-Density Devices.
  
  - This example has been tested with STMicroelectronics STM8L1528-EVAL 
    (STM8L15x High-Density devices) and STM8L1526-EVAL (STM8L15x Medium-Density and
    Low-Density devices) evaluation boards and can be easily tailored to any other
    development board.

  - STM8L1528-EVAL Set-up
     - Make sure that the LCD glass daughter board is mounted in IO position.
       For more details please refer to the evaluation board user manual.
     - Before running the example make sure that potentiometer RV3 is at left position.

  - STM8L1526-EVAL Set-up
     - Make sure that the LCD glass daughter board is mounted in IO position.
       For more details please refer to the evaluation board user manual.
     - Before running the example make sure that potentiometer RV2 is at right 
       position and the JP7 is in key position.
  
  @par How to use it ?

  In order to make the program work, you must do the following :

  - Copy all source files from this example folder to the template folder under
    Project\STM8L15x_StdPeriph_Template
  - Open your preferred toolchain 
  - Add the required file:
    - stm8_eval.c     (under Utilities\STM8_EVAL)
    - timing_delay.c      (under Utilities\Misc)
  - Rebuild all files and load your image into target memory
  - Run the example
  - The four LEDs are blinking
  - Press the key button to enter the CPU in halt mode, the four LEDs stop toggling
  - Rotate the potentiometer in order to make a rising edge
  - The CPU exits the halt mode and LEDs are blinking again



 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
