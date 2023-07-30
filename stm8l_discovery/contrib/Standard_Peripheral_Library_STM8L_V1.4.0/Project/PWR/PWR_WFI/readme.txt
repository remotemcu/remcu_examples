/** @page PWR_WFI Enter WFI mode and exit with EXTI Line or RTC wake-up interrupts
  
  @verbatim
  ******************** (C) COPYRIGHT 2013 STMicroelectronics *******************
  * @file    PWR/PWR_WFI/readme.txt 
  * @author  MCD Application Team
  * @version V1.5.2
  * @date    30-September-2014
  * @brief   Description of the PWR_WFI Example.
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

  This example shows how to enter the system to WFI mode and exit with EXTI
  Line or RTC wake-up interrupts.

  The system enters and exits WFI mode as following:
  After a delay from system start-up, the RTC is configured to generate a wake-
  up interrupt in 5 second then the system enters  WFI mode. 
  To wake-up from  WFI mode you have to push Key pushbutton, otherwise the RTC 
  will wake-up the system within 5 second.
  Then after a delay the system will enter again in  WFI mode and exit in the 
  way described above. This behavior is repeated in an infinite loop.

  Four leds are used to monitor the system state as following:
 - LED1 on: system in RUN mode
 - LED2 on: system in WFI mode
 - LED3 is toggled if Key pushbutton is pressed
 - LED4 is toggled if RTC wake-up interrupt is occurred


  @par Directory contents

  - PWR/PWR_WFI/main.c            Main file containing the "main" function
  - PWR/PWR_WFI/stm8l15x_conf.h   Library Configuration file
  - PWR/PWR_WFI/stm8l15x_it.c     Interrupt routines source
  - PWR/PWR_WFI/stm8l15x_it.h     Interrupt routines declaration


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
     - LD1..4
     - Key pushbutton

  - STM8L1526-EVAL Set-up
     - Make sure that the LCD glass daughter board is mounted in IO position.
       For more details please refer to the evaluation board user manual.
     - LD1..4
     - Key pushbutton
     - Make sure that JP7 jumper is in Key position.
  
  
  @par How to use it ?

  In order to make the program work, you must do the following :

  - Copy all source files from this example folder to the template folder under
    Project\STM8L15x_StdPeriph_Template
  - Open your preferred toolchain 
  - Add the required file
    - stm8_eval.c   (under Utilities\STM8_EVAL)
  - Rebuild all files and load your image into target memory
  - Run the example
  


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
