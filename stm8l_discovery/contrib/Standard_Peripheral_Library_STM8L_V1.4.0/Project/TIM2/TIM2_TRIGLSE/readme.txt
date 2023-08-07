/** @page TIM2_TRIGLSE Generate PWM signal using LSE clock source for TIM2 counter 

  @verbatim
  ******************** (C) COPYRIGHT 2013 STMicroelectronics *******************
  * @file    TIM2/TIM2_TRIGLSE/readme.txt
  * @author  MCD Application Team
  * @version V1.5.2
  * @date    30-September-2014
  * @brief   Description of the TIM2 external trigger mapped to LSE clock Example.
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

  This example explains how to configure TIM2 time base that works completely 
  independently of the system clock. This allows the scheduling of tasks without
  having to take into account the processor state (the processor may be stopped 
  or executing at low or full speed).
  For this purpose, the LSE clock can be internally mapped to the TIM2 ETR input.

  Moreover, this example shows how to configure the TIM2 channel 1 in PWM 
  (Pulse Width Modulation) mode and to be clocked with the LSE clock.
  The TIM2CLK frequency is set to the ETR div 4, the counter prescaler is 1 
  so the TIM2 counter clock is LSE / 4 = 32.768 KHz / 4 = 8.192 KHz.

  The TIM2 period is set to 7, so update event is generated each (7 + 1 ) / 8.192KHz ~ 1 ms
  TIM2 Channel 1 generates a PWM signal with a frequency equal to 1.024 KHz
  TIM2 pulse is set to 4, so the TIM2 Channel 1 generates a PWM signal with 
  a duty cycle equal to 50%:
  TIM2 Channel1 duty cycle = (TIM2 Pulse / (TIM2 Period + 1)) * 100 = 100 * (4 / (7 + 1)) = 50%

  @par Directory contents

  - TIM2/TIM2_TRIGLSE/main.c            Main file containing the "main" function
  - TIM2/TIM2_TRIGLSE/stm8l15x_conf.h   Library Configuration file
  - TIM2/TIM2_TRIGLSE/stm8l15x_it.c     Interrupt routines source
  - TIM2/TIM2_TRIGLSE/stm8l15x_it.h     Interrupt routines declaration


  @par Hardware and Software environment

    - This example runs on STM8L15x High-Density, Medium-Density Plus, Medium-Density
    and Low-Density Devices.

  - This example has been tested with STMicroelectronics STM8L1528-EVAL 
    (STM8L15x High-Density devices) and STM8L1526-EVAL (STM8L15x Medium-Density and
    Low-Density devices) evaluation boards and can be easily tailored to any other
    development board.

  - STM8L1528-EVAL & STM8L1526-EVAL Set-up
    - Make sure that the LCD glass daughter board is mounted in IO position.
      For more details please refer to the evaluation board user manual.
    - Connect TIM2 Channel 1 pin (PB0) to an oscilloscope


  @par How to use it ?

  In order to make the program work, you must do the following :

  - Copy all source files from this example folder to the template folder under
    Project\STM8L15x_StdPeriph_Template
  - Open your preferred toolchain 
  - Rebuild all files and load your image into target memory
  - Run the example



 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
