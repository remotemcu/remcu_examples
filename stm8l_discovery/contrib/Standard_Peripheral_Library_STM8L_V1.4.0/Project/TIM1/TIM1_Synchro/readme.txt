/** @page TIM1_Synchro Synchronization between TIM1 and TIM2 & TIM3 using TIM1 update event.

  @verbatim
  ******************** (C) COPYRIGHT 2013 STMicroelectronics *******************
  * @file    TIM1/TIM1_Synchro/readme.txt 
  * @author  MCD Application Team
  * @version V1.5.2
  * @date    30-September-2014
  * @brief   Description of the TIM1 synchronization with TIM2 and TIM3 Example.
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

  This example shows how to synchronize TIM1 and TIM2 & TIM3 using TIM1 update
  event.
  TIM1 is configured as Master Timer:
   - PWM Mode 2 is used
   - TIM1 Update event is used as Trigger Output

  TIM2 and TIM3 are slaves for TIM1:
   - PWM Mode 2
   - Gated mode: so start and stop of slaves counters are controlled by 
     the Master trigger output signal(TIM1 update event  .

  TIM1CLK is set to 2 MHz, the TIM1 Prescaler is equal to 15 so the TIM1 counter
  clock used is 2MHz / 15 + 1 =  125 000 Hz
  TIM1 channel 1 frequency = TIM1 counter clock / (TIM1_PERIOD + 1) = 125 000 / 256 
                           = 488.28 Hz
  TIM1 channel 1 duty cycle equal to 100 * TIM1_CCR1_VAL / (TIM1_PERIOD + 1) 
                                   = 100 * 127 / (255 + 1) = 50 %

  The TIM2 is running at:
  TIM2 channel 1 frequency = TIM1 channel 1 frequency / ((TIM2_PERIOD + 1)* (TIM1_REPTETION_COUNTER + 1))    
                           = 488.28 / (2 + 1) * (4 + 1))  
                           = 32.5 Hz
  TIM2 channel 1 duty cycle equal to 100 * TIM2_CCR1_VAL /(TIM2_PERIOD + 1)   
                                   = 100 * 1 / (2 + 1)   
                                   = 33.3%

  The TIM3 is running at:
  TIM3 channel 1 frequency = TIM1 channel 1 frequency / ((TIM3_PERIOD + 1) * (TIM1_REPTETION_COUNTER + 1))    
                           = 488.28 / ((3 + 1) * (4 + 1))    
                           = 24.41 Hz
  TIM3 channel 1 duty cycle equal to 100 * TIM3_CCR1_VAL /(TIM3_PERIOD + 1)   
                                   = 100 * 1 / (3 + 1)  
                                   = 25% 

  @par Directory contents

  - TIM1/TIM1_Synchro/main.c                   Main file containing the "main" function
  - TIM1/TIM1_Synchro/stm8l15x_conf.h          Library Configuration file
  - TIM1/TIM1_Synchro/stm8l15x_it.c            Interrupt routines source
  - TIM1/TIM1_Synchro/stm8l15x_it.h            Interrupt routines declaration


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
    - Connect TIM1 channel 1, TIM2 channel 1 and TIM3 channel 1 to an oscilloscope:
     - TIM1 Channel 1 pin (PD2)  
     - TIM2 Channel 1 pin (PB0)  
     - TIM3 Channel 1 pin (PB1)  

  
  @par How to use it ?

  In order to make the program work, you must do the following :

  - Copy all source files from this example folder to the template folder under
    Project\STM8L15x_StdPeriph_Template
  - Open your preferred toolchain 
  - Rebuild all files and load your image into target memory
  - Run the example



 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
