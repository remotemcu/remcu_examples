/** @page RTC_ChronoSubSecond Use RTC as a chronometer

  @verbatim
  ******************** (C) COPYRIGHT 2013 STMicroelectronics *******************
  * @file    RTC/RTC_ChronoSubSecond/readme.txt 
  * @author  MCD Application Team
  * @version V1.5.2
  * @date    30-September-2014
  * @brief   Description of the RTC Chronomter Example.
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
  calendar feature: sub seconds, seconds, minutes, hours to simulate a precise 
  chronometer with 9 record times possibilities, and Start/Pause/Resume actions.

  For this example an interactive human interface is developed using
  STM8L1528-EVAL’s LCD and Joystick to allow user to use Chronometer with real 
  time display.

  After startup, a default 00:00:00:000 chronometer counter is displayed on the 
  LCD, it corresponds to [Hours]:[minutes]:[seconds]:[milliseconds].
  User can manipulate the chronometer features using the Tamper, Key and 
  Joystick navigation buttons:
    - press Joystick SEL button to Start/Pause/Resume the chronometer counter
    - press Key button to store the current chrono Time and Rank, 9 records can 
      be performed.
    - press Joystick DOWN button to enter to the Recorded Times menu, where you 
      can navigate using the Joystick RIGHT/LEFT buttons.
    - press Joystick UP button to exit from the Recorded Times menu.
    - press Tamper button during 2sec to enter to the reset menu where you can 
      select using RIGHT/LEFT buttons to clear the counter and/or the recorded times.
      
    @note During Pause, Pressing on Tamper Button does not have any effect.

  @par Directory contents

  - RTC/RTC_ChronoSubSecond/main.c                   Main file
  - RTC/RTC_ChronoSubSecond/main.h                   Main file declaration
  - RTC/RTC_ChronoSubSecond/stm8l15x_conf.h          Library Configuration file
  - RTC/RTC_ChronoSubSecond/stm8l15x_it.c            Interrupt routines source
  - RTC/RTC_ChronoSubSecond/stm8l15x_it.h            Interrupt routines declaration


  @par Hardware and Software environment

  - This example runs on STM8L15x High-Density devices.
  
  - This example has been tested with STMicroelectronics STM8L1528-EVAL 
    evaluation board and can be easily tailored to any other development board.
    
  
  - STM8L1528-EVAL Set-up
     - Make sure that the LCD glass daughter board is mounted in IO position.
       For more details please refer to the evaluation board user manual.
     - Joystick, Tamper and Key buttons
     - LED1..4
     - Dot matrix LCD
     - External 32.768kHz LSE clock (X1)  
     - Make sure that SB9 (Solder Bridge) is in position 2-3.
            
      
  @par How to use it ?

  In order to make the program work, you must do the following :

  - Copy all source files from this example folder to the template folder under
    Project\STM8L15x_StdPeriph_Template
  - Open your preferred toolchain 
  - Choose STM8L15X_HD(STM8L15xx8_16xx8) project
    - Add the reiquired files:
       - stm8_eval.c  (Utilities\STM8_EVAL)
       - stm8_eval_lcd.c   (Utilities\STM8_EVAL\Common)  
  - Rebuild all files and load your image into target memory
  - Run the example
    


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
