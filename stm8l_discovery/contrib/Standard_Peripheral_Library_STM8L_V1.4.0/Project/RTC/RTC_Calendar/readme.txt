/** @page RTC_Calendar Use RTC for Time, Date and Alarm management

  @verbatim
  ******************** (C) COPYRIGHT 2013 STMicroelectronics *******************
  * @file    RTC/RTC_Calendar/readme.txt 
  * @author  MCD Application Team
  * @version V1.5.2
  * @date    30-September-2014
  * @brief   Description of the RTC Calendar Example.
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

  This example provides a short description of how to use the RTC peripheral’s
  calendar features: seconds, minutes, hours, day, date, month, and year. 
  In addition, a very basic Alarm configuration is used to facilitate the RTC 
  peripheral’s Alarm features use.

  For this example an interactive human interface is developed using STM8L152x-EVAL’s
  LCD and Joystick to allow user to configure the RTC calendar with real time display.

  After startup, a default date and time are displayed on the LCD. User can 
  modify the date, time and alarm using the Joystick navigation buttons:
    - press Joystick LEFT button to configure the Date
    - press Joystick RIGHT button to configure the Time
    - press Key button to configure the alarm. When an alarm occurs a message
      is displayed on the LCD and four leds toggles for a second.

  Once you press a button, from the list above, a message will be displayed on
  the LCD indicating the selected calendar field (date, time or alarm, in the
  first line of the LCD   and the current value to modify (in the second line
  of the LCD  .
  This value will be modified, digit by digit, according to value entered by 
  user with Joystick UP, DOWN and SEL buttons:  
    - the digit to modify will blink for a moment, then
    - press joystick UP to increment this digit value 
    - press joystick DOWN to decrement this digit value
    - after selecting the digit value press SEL, the cursor then jumps 
    automatically to the next digit.
    Proceed this way until the entire value digits are entered.


  @par Directory contents

  - RTC/RTC_Calendar/main.c                   Main file 
  - RTC/RTC_Calendar/main.h                   Main file declaration
  - RTC/RTC_Calendar/stm8l15x_conf.h          Library Configuration file
  - RTC/RTC_Calendar/stm8l15x_it.c            Interrupt routines source
  - RTC/RTC_Calendar/stm8l15x_it.h            Interrupt routines declaration


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
     - Joystick and Key buttons
     - LED1..4
     - Dot matrix LCD
     - External 32.768kHz LSE clock (X1)  
     - Make sure that SB9 (Solder Bridge)is in position 2-3.
     
     
  - STM8L1526-EVAL Set-up
     - Make sure that the LCD glass daughter board is mounted in IO position.
       For more details please refer to the evaluation board user manual.
     - Joystick and Key buttons
     - Make sure that JP7 is in key position.
     - LED1..4
     - Dot matrix LCD
     - External 32.768kHz LSE clock (X2)  
     

  @par How to use it ?

  In order to make the program work, you must do the following :

  - Copy all source files from this example folder to the template folder under
    Project\STM8L15x_StdPeriph_Template
  - Open your preferred toolchain 
  - Add the required files:
       - stm8_eval.c   (under Utilities\STM8_EVAL)
       - stm8_eval_lcd.c  (under Utilities\STM8_EVAL\Common)  
  - Rebuild all files and load your image into target memory
  - Run the example



 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
