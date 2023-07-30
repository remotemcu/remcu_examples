/** @page LCD_SegmentsDrive Use the STM8L15x embedded LCD controller to drive an glass LCD (7 digit).

  @verbatim
  ******************** (C) COPYRIGHT 2013 STMicroelectronics *******************
  * @file    LCD/LCD_SegmentsDrive/readme.txt 
  * @author  MCD Application Team
  * @version V1.5.2
  * @date    30-September-2014
  * @brief   Description of the LCD segmens drive Example.
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

   This example provides a description of how to use the STM8L15x LCD embedded 
   controller to drive the Pacific Display LCD glass mounted on STM8L1528-EVAL
   and STM8L1526-EVAL boards.

   The first step consists in displaying "STM8L" string on LCD glass in scrolling mode.
   Then, "LP MODE" string is blinking on LCD glass before entering in HALT mode, 
   When the MCU enters in Halt mode the " HALT  " string is displayed on LCD 
   glass. 
   After 10 seconds the MCU wakes up from HALT mode (by RTC wakeup interrupt   
   and continues processing.
   These Steps are repeated in infinite loop.
   
   On the STM8L1528-EVAL, In addition to the text zone the LCD Glass embeds 
   more features(Matrix zone, Temperature indicator, Sign, ST Logo...   all these
   predefined characters are permanently displayed during the execution. 
 
   The LCD is configured as follow:
     - clock source = LSE (32.768 KHz)  
     - Voltage source = Internal
     - Prescaler = 2
     - Divider = 18 (16 + 2     
     - Mode = 1/4 Duty, 1/3 Bias
     - LCD frequency = (clock source * Duty) / (Prescaler * Divider)  
                     = 228 Hz ==> Frame frequency = 57 Hz


  @par Directory contents

  - LCD/LCD_SegmentsDrive/main.c            Main file containing the "main" function
  - LCD/LCD_SegmentsDrive/stm8l15x_conf.h   Library Configuration file
  - LCD/LCD_SegmentsDrive/stm8l15x_it.c     Interrupt routines source
  - LCD/LCD_SegmentsDrive/stm8l15x_it.h     Interrupt routines declaration


  @par Hardware and Software environment

    - This example runs on STM8L15x High-Density, Medium-Density Plus and Medium-Density
     Devices.
  
  - This example has been tested with STMicroelectronics STM8L1528-EVAL 
    (STM8L15x High-Density devices) and STM8L1526-EVAL (STM8L15x Medium-Density 
    devices) evaluation boards and can be easily tailored to any other
    development board.

  - STM8L1528-EVAL Set-up
     - LCD glass daughter board (MB905)  
     - Make sure that the LCD glass daughter board (MB905) is mounted in LCD 
       position.
       For more details please refer to the evaluation board user manual.

  - STM8L1526-EVAL Set-up
     - LCD glass daughter board (MB821)  
     - Make sure that the LCD glass daughter board (MB821) is mounted in LCD 
       position.
       For more details please refer to evaluation board user manual.


  @par How to use it ?
  In order to make the program work, you must do the following :

  - Copy all source files from this example folder to the template folder under
    Project\STM8L15x_StdPeriph_Template
  - Open your preferred toolchain 
  - Add the required files :
    - stm8_eval.c (under Utilities\STM8_EVAL)
    - stm8l1528_eval_glass_lcd.c (if STM8L1528-EVAL is used, this file is under
                                  Utilities/STM8_EVAL/STM8L1528_EVAL  
   or stm8l1526_eval_glass_lcd.c (if STM8L1526-EVAL is used, this file is under
                                  Utilities/STM8_EVAL/STM8L1526_EVAL  
  - Rebuild all files and load your image into target memory
  - Run the example


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
