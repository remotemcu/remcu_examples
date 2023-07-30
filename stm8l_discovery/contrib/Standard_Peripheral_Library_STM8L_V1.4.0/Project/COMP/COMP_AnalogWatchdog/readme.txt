/** @page COMP_AnalogWatchdog  Use COMP1 and COMP2 in window mode to make an analog watchdog.

  @verbatim
  ******************** (C) COPYRIGHT 2013 STMicroelectronics *******************
  * @file    COMP/COMP_AnalogWatchdog/readme.txt 
  * @author  MCD Application Team
  * @version V1.5.2
  * @date    30-September-2014
  * @brief   Description of the COMP Analog Watchdog Example.
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

  This example shows how to make an analog watchdog using the COMPs peripherals
  in window mode:
   - The upper threshold is set to VREFINT = 1.22V
   - The lower threshold is set to VREFINT / 4 = 1.22V = 0.305V
   - PD1 is configured as COMP1 and COMP2 non inverting input (Connect external voltage to PD1)  

  If the input voltage is above the higher threshold, LD3 and LD1 are turned On.
  If the input voltage is under the lower threshold, LD2 and LD4 are turned On.
  If the input voltage is within the thresholds, the LD1, LD2, LD3 and LD4 are Off 
  and MCU is in halt mode. MCU wakes-up from halt if input voltage crosses the 
  high threshold or the low threshold.

  @par Directory contents

  - COMP\COMP_AnalogWatchdog\main.c            Main file containing the "main" function
  - COMP\COMP_AnalogWatchdog\stm8l15x_conf.h   Library Configuration file
  - COMP\COMP_AnalogWatchdog\stm8l15x_it.c     Interrupt routines source
  - COMP\COMP_AnalogWatchdog\stm8l15x_it.h     Interrupt routines declaration


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
     - Connect external voltage to PD1

  - STM8L1526-EVAL Set-up
     - Make sure that the LCD glass daughter board is mounted in IO position.
       For more details please refer to the evaluation board user manual.
     - Connect external voltage to PD1
     
  @par How to use it ?

  In order to make the program work, you must do the following :

  - Copy all source files from this example folder to the template folder under
    Project\STM8L15x_StdPeriph_Template
  - Open your preferred toolchain 
  - Add the required file:
    - stm8_eval.c   (under Utilities\STM8_EVAL)
  - Rebuild all files and load your image into target memory
  - Run the example



 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
