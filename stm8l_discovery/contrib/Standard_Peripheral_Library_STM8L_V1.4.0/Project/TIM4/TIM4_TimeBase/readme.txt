/** @page TIM4_TimeBase  Use the TIM4 peripheral to generate 1 ms time base using TIM4 update interrupt 
 
  @verbatim
  ******************** (C) COPYRIGHT 2013 STMicroelectronics *******************
  * @file    TIM4/TIM4_TimeBase/readme.txt 
  * @author  MCD Application Team
  * @version V1.5.2
  * @date    30-September-2014
  * @brief   Description of the TIM4 Time Base Example.
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

  This example provides a short description of how to use the TIM4 peripheral 
  to generate time base equal to 1 ms. In this example a "Delay" function is 
  implemented based on the TIM4 time base.
  The four LEDs LD1..LD4 toggle with a timing defined by the Delay function.

  The TIM4CLK frequency is 16MHz, the Prescaler is 128 so the TIM4 counter 
  clock is 125 KHz.
  The TIM4 is running at TIM4 frequency = TIM4 counter clock / (TIM4_PERIOD + 1)   
                                        = 125000 / 125 = 1 KHz.
  So the TIM4 generates an Update Interrupt each 1 / 1000 = 1 ms.


  @par Directory contents

  - TIM4/TIM4_TimeBase/main.c             Main file containing the "main" function
  - TIM4/TIM4_TimeBase/stm8l15x_conf.h    Library Configuration file
  - TIM4/TIM4_TimeBase/stm8l15x_it.c      Interrupt routines source
  - TIM4/TIM4_TimeBase/stm8l15x_it.h      Interrupt routines declaration


  @par Hardware and Software environment

  - This example runs on STM8L15x High-Density, Medium-Density Plus, Medium-Density
    and Low-Density Devices.

  - This example has been tested with STMicroelectronics STM8L1528-EVAL 
    (STM8L15x High-Density devices) and STM8L1526-EVAL (STM8L15x Medium-Density and
    Low-Density devices) evaluation boards and can be easily tailored to any other
    development board.

  - STM8L1526-EVAL and STM8L1528-EVAL Set-up
     - Make sure that the LCD glass daughter board is mounted in IO position.
      For more details please refer to the evaluation board user manual.
     - Four leds LD1..LD4


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
