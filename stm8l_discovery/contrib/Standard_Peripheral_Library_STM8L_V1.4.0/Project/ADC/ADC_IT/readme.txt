/** @page ADC_IT Use ADC to convert continuously Channel 3 in Interrupt mode
  
  @verbatim
  ******************** (C) COPYRIGHT 2013 STMicroelectronics *******************
  * @file    ADC/ADC_IT/readme.txt 
  * @author  MCD Application Team
  * @version V1.5.2
  * @date    30-September-2014
  * @brief   Description of the ADC Interrupt Example.
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

  This example provides a short description of how to use the ADC peripheral
  to convert ADC Channel 3 input voltage in Interrupt mode.
  The ADC channel 3 converted voltage value is displayed on LCD.

  In the STM8L1528-EVAL board, ADC Channel 3 is connected to RV3 Adjuster. 
  Turning RV3 adjuster provides a voltage which can vary from 0 Volt to 3.3 
  Volt.
  In the STM8L1526-EVAL board, ADC Channel 3 is connected to RV2 Adjuster. 
  Turning RV2 adjuster provides a voltage which can vary from 0 Volt to 3.3 
  Volt.

  @par Directory contents

  - ADC/ADC_IT/main.c                   Main file containing the "main" function
  - ADC/ADC_IT/stm8l15x_conf.h          Library Configuration file
  - ADC/ADC_IT/stm8l15x_it.c            Interrupt routines source
  - ADC/ADC_IT/stm8l15x_it.h            Interrupt routines declaration


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
     - RV3 adjuster
     - LCD display.

  - STM8L1526-EVAL Set-up
     - Make sure that the LCD glass daughter board is mounted in IO position.
     For more details please refer to the evaluation board user manual.
     - RV2 adjuster
     - LCD display.


  @par How to use it ?

  In order to make the program work, you must do the following :

  - Copy all source files from this example folder to the template folder under
    Project\STM8L15x_StdPeriph_Template
  - Open your preferred toolchain 
  - Add the required file:
    - stm8_eval.c   (under Utilities\STM8_EVAL)
    - stm8_eval_lcd.c   (under Utilities\STM8_EVAL\Common)
    - timing_delay.c      (under Utilities\Misc)
  - Rebuild all files and load your image into target memory
  - Run the example
  - Turn RV Adjuster to provide a varing voltage (from 0 Volt to 3.3 Volt  . The 
    ADC Channel 3 converted value is displayed on LCD.
  


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
