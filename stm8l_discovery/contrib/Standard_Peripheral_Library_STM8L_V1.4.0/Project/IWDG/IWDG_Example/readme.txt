/** @page IWDG_Example IWDG counter reload and simulation of IWDG reset

  @verbatim
  ******************** (C) COPYRIGHT 2013 STMicroelectronics *******************
  * @file    IWDG/IWDG_Example/readme.txt
  * @author  MCD Application Team
  * @version V1.5.2
  * @date    30-September-2014
  * @brief   Description of the IWDG example.
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

 This example shows how to update at regular period the IWDG reload counter and 
 how to simulate a software fault generating an MCU IWDG reset on expiry of a 
 programmed time period.

 The IWDG timeout is set to 214 ms (the timeout may varies due to LSI frequency 
 dispersion).

 First, the TIM2 timer is configured to measure the LSI frequency as the 
 LSI is internally connected to it, in order to adjust the IWDG clock.
 The IWDG reload counter is configured to obtain 214ms according to the 
 measured LSI frequency.

 The Key button is used to generate an interrupt that will simulate a software 
 failure by pressing it.
 In the ISR, a trap instruction is executed generating a TRAP interrupt containing
 an infinite loop and preventing to return to main program (the IWDG reload counter 
 is no more refreshed).
 As a result, when the IWDG counter reaches 00h, the IWDG reset occurs.
 If the IWDG reset is generated, after the system resumes from reset, LED1 turns on.

 If the Key button is not pressed, the IWDG counter is refreshed in an infinite loop
 and there is no IWDG reset.
  

  @par Directory contents

  - IWDG/IWDG_Example/main.c             Main file containing the "main" function
  - IWDG/IWDG_Example/stm8l15x_conf.h    Library Configuration file
  - IWDG/IWDG_Example/stm8l15x_it.c      Interrupt routines source 
  - IWDG/IWDG_Example/stm8l15x_it.h      Interrupt routines declaration


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
     - leds LD1 and Key push button are used.

  - STM8L1526-EVAL Set-up
     - Make sure that the LCD glass daughter board is mounted in IO position.
       For more details please refer to the evaluation board user manual.
     - leds LD1 and Key push button are used.
     - Make sure that JP7 is in key position.


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
