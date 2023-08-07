/** @page RTC_Tamper1Detection Use RTC Tamper1 Filter feature

  @verbatim
  ******************** (C) COPYRIGHT 2013 STMicroelectronics *******************
  * @file    RTC/RTC_Tamper1Detection/readme.txt 
  * @author  MCD Application Team
  * @version V1.5.2
  * @date    30-September-2014
  * @brief   Description of the RTC tamper 1 detection Example.
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
  Tamper feature : sample duration, filter, number of samples to wait for.


  After startup, 2 counters (00:000  00:000   are displayed on the LCD. Both 
  are composed of 2 fields [seconds on 2 digits]:[milliseconds on 3 digits].
  The first counter is used to store the time when the tamper event starts.
  To modify it you have to press the KEY button. 
  The second counter is used to display the current time (seconds and 
  milliseconds fields only  . 
  
  User can manipulate the application using the Tamper, Key and Joystick 
  navigation buttons:
    - Press Joystick UP or DOWN buttons to increase or decrease the delay 
      between the Tamper 1 start event and the Tamper 1 Interrupt. 
      Four configurations can be selected: 0s / 1s / 3s / 7s (see note1  .
    - Press Key button to store the current Time (see note2  .
    - press Tamper button and keep it pressed during the delay (selected using 
      Joystick UP or DOWN buttons  .
      After the selected delay passes, LED3 is On, the Tamper Interrupt occurs 
      and the MCU enters in the halt mode;

  To exit from the halt mode, press Key button (Joystick UP or DOWN buttons can 
  also be used  . When CPU exit from Halt mode, LED3 is off.

  User can press, on the same time, Key button and Tamper button; 
  and keep Tamper button pressed until LED3 is On (Tamper1 Interrupt occurs  , 
  then user can check the performed delay between the tamper 1 start event and 
  the Tamper Interrupt .

  @note note1: When selecting 3s of filter delay, on pressing on Tamper Button 
               tamper interrupt will occur a period between 3s:000 and 3s:999

  @note note2: User can also connect Key Button pin (PH.6   to Tamper Button pin
               (PI.0   to avoid pressing Key and Tamper buttons at the same time. 

  @par Directory contents

  - RTC/RTC_Tamper1Detection/main.c            Main file
  - RTC/RTC_Tamper1Detection/main.h            Main file declaration
  - RTC/RTC_Tamper1Detection/stm8l15x_conf.h   Library Configuration file
  - RTC/RTC_Tamper1Detection/stm8l15x_it.c     Interrupt routines source
  - RTC/RTC_Tamper1Detection/stm8l15x_it.h     Interrupt routines declaration


  @par Hardware and Software environment

  - This example runs on STM8L15x High-Density devices.
  
  - This example has been tested with STMicroelectronics STM8L1528-EVAL
   evaluation board and can be easily tailored to any other development board.

  - STM8L1528-EVAL Set-up
     - Make sure that the LCD glass daughter board is mounted in IO position.
       For more details please refer to the evaluation board user manual.
     - Joystick, Tamper and Key buttons
     - LED1 and LED3
     - Dot matrix LCD
     - External 32.768kHz LSE clock (X1)  
     - Make sure that SB9 (Solder Bridge) is in position 2-3.
              

  @par How to use it ?

  In order to make the program work, you must do the following :

  - Copy all source files from this example folder to the template folder under
    Project\STM8L15x_StdPeriph_Template
  - Open your preferred toolchain 
  - Choose STM8L15X_HD(STM8L15xx8_16xx8) project
    - Add the required files following files to the user folder in the project
      - stm8_eval.c   (under Utilities\STM8_EVAL)
      - stm8_eval_lcd.c   (under Utilities\STM8_EVAL\Common)  
  - Rebuild all files and load your image into target memory
  - Run the example
  - press, on the same time, Key button and Tamper button; and keep Tamper 
    button pressed until LED3 is On.
    


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
