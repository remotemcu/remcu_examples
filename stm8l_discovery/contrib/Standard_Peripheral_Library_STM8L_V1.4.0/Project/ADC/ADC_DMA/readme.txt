/** @page ADC_DMA Use ADC to convert continuously Channel 3 and Channel 24 in scan mode using DMA
  
  @verbatim
  ******************** (C) COPYRIGHT 2013 STMicroelectronics *******************
  * @file    ADC/ADC_DMA/readme.txt 
  * @author  MCD Application Team
  * @version V1.5.2
  * @date    30-September-2014
  * @brief   Description of the ADC DMA Example.
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
  to convert two ADC Channels input voltage using DMA.
  The ADC channel 3 and channel 24 converted voltage values are displayed on LCD.

  In the STM8L1528-EVAL board, ADC Channel 3 is connected to RV3 Adjuster and ADC 
  Channel 24 is connected to BNC connector CN2.
  In the STM8L1526-EVAL board, ADC Channel 3 is connected to RV2 Adjuster and ADC 
  Channel 24 is connected to BNC connector CN15.   

  Turning RV adjuster provides a voltage which can vary from 0 Volt to 3.3 Volt.
  The BNC connector may be connected to a DC Power supply voltage to provide
  a voltage from 0 Volt to 3.3 Volt.
  
  @par Directory contents

  - ADC/ADC_DMA/main.c             Main file containing the "main" function
  - ADC/ADC_DMA/stm8l15x_conf.h    Library Configuration file
  - ADC/ADC_DMA/stm8l15x_it.c      Interrupt routines source
  - ADC/ADC_DMA/stm8l15x_it.h      Interrupt routines declaration


  @par Hardware and Software environment

  - This example runs on STM8L15x High-Density, Medium-Density Plus and 
  Medium-Density Devices.
  
  - This example has been tested with STMicroelectronics STM8L1528-EVAL 
    (STM8L15x High-Density devices) and STM8L1526-EVAL (STM8L15x Medium-Density)
    evaluation boards and can be easily tailored to any other development board.

  - STM8L1528-EVAL Set-up
     - Make sure that the LCD glass daughter board is mounted in IO position.
     For more details please refer to the evaluation board user manual.
     - RV3 adjuster
     - BNC connector CN2
     - Dot matrix LCD
    
  - STM8L1526-EVAL Set-up
     - Make sure that the LCD glass daughter board is mounted in IO position.
     For more details please refer to the evaluation board user manual.
     - RV2 adjuster
     - BNC connector CN15
     - Dot matrix LCD
     - Before Running the example make sure that jumper JP10 is in BNC position.
  
  @par How to use it ?

  In order to make the program work, you must do the following :

  - Copy all source files from this example folder to the template folder under
    Project\STM8L15x_StdPeriph_Template
  - Open your preferred toolchain 
  - Add the required files:
    - stm8_eval.c       (under Utilities\STM8_EVAL)
    - stm8_eval_lcd.c   (under Utilities\STM8_EVAL\Common)
  - Rebuild all files and load your image into target memory
  - Run the example
  - Turn RV Adjuster to provide a varing voltage (from 0 Volt to 3.3 Volt). The 
    ADC Channel 3 converted value is displayed on LCD.
  - Connect a Power supply varing voltage (from 0 Volt to 3.3 Volt) to the BNC 
    connector. The ADC Channel 24 converted value is displayed on LCD.
  
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
