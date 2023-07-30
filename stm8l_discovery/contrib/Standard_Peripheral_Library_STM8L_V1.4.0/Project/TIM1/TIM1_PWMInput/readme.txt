/** @page TIM1_PWMInput Use the TIM1 peripheral to measure the frequency and duty cycle of an external signal

  @verbatim
  ******************** (C) COPYRIGHT 2013 STMicroelectronics *******************
  * @file    TIM1/TIM1_PWMInput/readme.txt 
  * @author  MCD Application Team
  * @version V1.5.2
  * @date    30-September-2014
  * @brief   Description of the TIM1 PWM Input Example.
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

  This example shows how to configure the TIM1 peripheral to measure 
  the frequency and the duty cycle of an external signal.
  TIM1CLK is set to 16 MHz, the TIM1 Prescaler is equal to 1 so the minimum 
  frequency value to measure is 245 Hz.
  The TIM1 Channel 1 is configured in PWM Input Mode: the external signal is 
  connected to TIM1 Channel1.
  To measure the frequency we use the TIM1 CC1 interrupt request, and the 
  frequency of the external signal is computed in the TIM1_CAP_IRQHandler
  routine.
  The frequency and the duty cycle of the external signal are stored in 
  SignalFrequency and SignalDutyCycle variables:
  SignalFrequency = TIM1CLK / TIM1_CCR1 in Hz,
  SignalDutyCycle = (TIM1_CCR2 * 100) / (TIM1_CCR1) in %.

  @par Directory contents

  - TIM1/TIM1_PWMInput/main.c              Main file containing the "main" function
  - TIM1/TIM1_PWMInput/stm8l15x_conf.h     Library Configuration file
  - TIM1/TIM1_PWMInput/stm8l15x_it.c       Interrupt routines source
  - TIM1/TIM1_PWMInput/stm8l15x_it.h       Interrupt routines declaration


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
     - Connect the external signal to measure to TIM1 CH1 pin (PD.2  


  @par How to use it ?

  In order to make the program work, you must do the following :

  - Copy all source files from this example folder to the template folder under
    Project\STM8L15x_StdPeriph_Template
  - Open your preferred toolchain 
  - Rebuild all files and load your image into target memory
  - Run the example
   - Connect the external signal to measure to the TIM1 Channel1 pin (PD.2  
   - Add "SignalFrequency" and "SignalDutyCycle" variables in watch to display 
     the frequency and duty cycle of the external signal.
     (declared in TIM1/TIM1_PWMInput/stm8l15x_it.c  
  


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
