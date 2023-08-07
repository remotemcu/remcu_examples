/** @page TIM1_ComplementarySignals Generating three complementary PWM signals 
      with dead time insertion, break capability and lock option using TIM1

  @verbatim
  ******************** (C) COPYRIGHT 2013 STMicroelectronics *******************
  * @file    TIM1/TIM1_ComplementarySignals/readme.txt
  * @author  MCD Application Team
  * @version V1.5.2
  * @date    30-September-2014
  * @brief   Description of the TIM1 complementary signals with dead time, break and lock Example.
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

  This example shows how to configure the TIM1 peripheral to generate three
  complementary signals, to insert a defined dead time value, to use the
  break feature and to lock the desired parameters.

  TIM1CLK is set to 2 MHz, the TIM1 Prescaler is equal to 0 so the TIM1 counter
  clock used is 2 MHz.
  TIM1 frequency is defined as follow:
  TIM1 channels frequency = TIM1 counter clock / (TIM1_PERIOD + 1) = 30.52 Hz.

  The Three Duty cycles are computed as the following description:

  TIM1 Channel1 duty cycle = CCR1_VAL / (TIM1_PERIOD + 1) = 50%
  TIM1 Channel1N duty cycle = (TIM1_PERIOD - CCR1_VAL) / (TIM1PERIOD + 1) = 50%

  TIM1 Channel2 duty cycle = CCR2_VAL / (TIM1_PERIOD + 1) = 25%
  TIM1 Channel2N duty cycle = (TIM1_PERIOD - CCR2_VAL) / (TIM1PERIOD + 1) = 75%

  TIM1 Channel3 duty cycle = CCR3_VAL / TIM1_PERIOD = 12.5%
  TIM1 Channel3N duty cycle = (TIM1_PERIOD - CCR3_VAL) / (TIM1PERIOD + 1) = 87.5%

  The break Polarity is defined at low level so applying a low level on PD6
 sets the non-inverted channels and inverted channels in idle state: high level.

  A dead time of 58.5 µs is inserted between each two complementary channels:
  dead time = DEADTIME / TIM1CLK = 117 / 2 MHz = 58.5 µs.

  The selected lock level is level 1 so once programmed channels idle state,
  break enable and polarity bits can no longer be written.

  The TIM1 waveform can be displayed using an oscilloscope.

  @par Directory contents

  - TIM1_ComplementarySignals/main.c                   Main file containing the "main" function
  - TIM1_ComplementarySignals/stm8l15x_conf.h          Library Configuration file
  - TIM1_ComplementarySignals/stm8l15x_it.c            Interrupt routines source
  - TIM1_ComplementarySignals/stm8l15x_it.h            Interrupt routines declaration


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
    - Connect TIM1 channels pins to an oscilloscope as follows:
     - TIM1 Channel 1   pin (PD2)  
     - TIM1 Channel 1N  pin (PD7)  
     - TIM1 Channel 2   pin (PD4)  
     - TIM1 Channel 2N  pin (PE1)  
     - TIM1 Channel 3   pin (PD5)  
     - TIM1 Channel 3N  pin (PE2)  
    - Make sure that the MicroSD card is disconnected before running 
        the example (in case of using STM8L1526-EVAL).


  @par How to use it ?

  In order to make the program work, you must do the following :

  - Copy all source files from this example folder to the template folder under
    Project\STM8L15x_StdPeriph_Template
  - Open your preferred toolchain 
  - Rebuild all files and load your image into target memory
  - Run the example
    - Connect TIM1 channels pins to an oscilloscope
    - You can apply a low level on pin PD6 to set channels in idle state.
    


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
