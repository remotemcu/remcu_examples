/** @page DMA_ADCtoTIM Use DMA to transfer continuously data received by ADC1 to TIM1

  @verbatim
  ******************** (C) COPYRIGHT 2013 STMicroelectronics *******************
  * @file    DMA/DMA_ADCtoTIM/readme.txt 
  * @author  MCD Application Team
  * @version V1.5.2
  * @date    30-September-2014
  * @brief   Description of the DMA's ADC to TIM example.
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

  This example provides a short description of how to use the DMA peripheral in 
  circular mode to transfer data from ADC peripheral to memory and from memory
  to TIM1 peripheral.
  In the STM8L1528-EVAL board, ADC Channel 3 is connected to RV3 Adjuster. 
  Turning RV3 adjuster provides a value which can vary from 0 to 0xFFF.
  In the STM8L1526-EVAL board, ADC Channel 3 is connected to RV2 Adjuster.
  Turning RV2 adjuster provides a value which can vary from 0 to 0xFFF.
  
  When the user turns the RV adjuster, the converted data is transferred using
  DMA to a specific RAM Buffer.
  When the TIM1 update event occurs, the RAM Buffer is transferred using DMA
  to the CCR1 registers to update the TIM1 channel 1 PWM duty cycle.
  TIM1 Channel 1 is configured as PWM output with a fixed autoreload value = 0xFFF.
  when TIM1 CCR1 registers are equal to 0 : the TIM1 Channel1 duty cycle is 0%.
  when TIM1 CCR1 registers are equal to 0xFFF : the TIM1 Channel1 duty cycle is 100%.
  In the STM8L1528-EVAL and STM8L1526-EVAL boards, TIM1 channel 1 is connected
  to a bicolor led which switch from green color (duty cycle = 0%) to red color 
  (duty cycle = 100%) depending on the value of TIM1 CCR1 registers.
  
  
  @par Directory contents

  - DMA/DMA_ADCtoTIM/main.c             Main file containing the "main" function
  - DMA/DMA_ADCtoTIM/stm8l15x_conf.h    Library Configuration file
  - DMA/DMA_ADCtoTIM/stm8l15x_it.c      Interrupt routines source
  - DMA/DMA_ADCtoTIM/stm8l15x_it.h      Interrupt routines declaration


  @par Hardware and Software environment

    - This example runs on STM8L15x High-Density, Medium-Density Plus and Medium-Density
      Devices.
  
  - This example has been tested with STMicroelectronics STM8L1528-EVAL 
    (STM8L15x High-Density devices) and STM8L1526-EVAL (STM8L15x Medium-Density 
    devices) evaluation boards and can be easily tailored to any other
    development board.

  - STM8L1528-EVAL Set-up 
     - Make sure that the LCD glass daughter board is mounted in IO position.
       For more details please refer to the evaluation board user manual.
     - RV3 adjuster
     - Bicolor Led.

  - STM8L1526-EVAL Set-up
     - Make sure that the LCD glass daughter board is mounted in IO position.
       For more details please refer to the evaluation board user manual.
     - RV2 adjuster
     - Bicolor Led.
    
  @par How to use it ?

  In order to make the program work, you must do the following :

  - Copy all source files from this example folder to the template folder under
    Project\STM8L15x_StdPeriph_Template
  - Open your preferred toolchain 
  - Rebuild all files and load your image into target memory
  - Run the example
  - Turn RV Adjuster to provide a varying voltage (from 0 Volt to 3.3 Volt). The
    ADC Channel 3 converted value is displayed via bicolor led which switch from
    green color (duty cycle = 0% = 0V   to red color (duty cycle = 100% = 3.3V).

  


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
