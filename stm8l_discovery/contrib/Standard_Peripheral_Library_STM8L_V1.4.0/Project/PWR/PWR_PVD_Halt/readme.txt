/** @page PWR_PVD_Halt Use PVD to monitor VDD power supply and put MCU in Halt mode when VDD < PVD level 

  @verbatim
  ******************** (C) COPYRIGHT 2013 STMicroelectronics *******************
  * @file    PWR/PWR_PVD_Halt/readme.txt 
  * @author  MCD Application Team
  * @version V1.5.2
  * @date    30-September-2014
  * @brief   Description of the PWR_PVD_Halt Example.
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

  This example provides a short description of how to enter MCU in Halt mode and 
  exit through PVD interrupt. 

  For the purpose of this example the MCU VDD is connected to adjustable DC power
  supply (VDD ADJ on STM8L1526-EVAL and STM8L1528-EVAL) and the PVD level is set to 2.85V.

  Once the PVD is enabled, it starts monitoring VDD level:
   - When VDD level goes below 2.85V, the PVD interrupt is generated and in the
     corresponding ISR a message is displayed on the LCD then the MCU enters Halt mode.
   - When the VDD level goes above 2.85V, the PVD interrupt is generated which
     causes the MCU to exit from Halt mode.
     This behavior is repeated in an infinite loop.

  On STM8L1528-EVAL board, use the RV2 potentiometer to adjust VDD level.
  On STM8L1526-EVAL board, use the RV1 potentiometer to adjust VDD level.


  @par Directory contents

  - PWR/PWR_PVD_Halt/main.c                   Main file containing the "main" function
  - PWR/PWR_PVD_Halt/stm8l15x_conf.h          Library Configuration file
  - PWR/PWR_PVD_Halt/stm8l15x_it.c            Interrupt routines source
  - PWR/PWR_PVD_Halt/stm8l15x_it.h            Interrupt routines declaration


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
     - Use potentiometer RV2 to adjust VDD.
     - Make sure to set jumper JP14 in ADJ position (1-2) to connect the VDD
       to adjustable DC power supply
     - Dot matrix LCD

  - STM8L1526-EVAL Set-up
     - Make sure that the LCD glass daughter board is mounted in IO position.
       For more details please refer to the evaluation board user manual.
     - Use potentiometer RV1 to adjust VDD.
     - Make sure to set jumper JP1 in ADJ position (1-2) to connect the VDD
       to adjustable DC power supply
     - Dot matrix LCD

  @par How to use it ?

  In order to make the program work, you must do the following :

  - Copy all source files from this example folder to the template folder under
    Project\STM8L15x_StdPeriph_Template
  - Open your preferred toolchain 
  - Add the required files :
    - stm8_eval.c   (under Utilities\STM8_EVAL)
    - stm8_eval_lcd.c    (under Utilities\STM8_EVAL\Common)
    - timing_delay.c      (under Utilities\Misc)
  - Rebuild all files and load your image into target memory
  - Run the example



 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
