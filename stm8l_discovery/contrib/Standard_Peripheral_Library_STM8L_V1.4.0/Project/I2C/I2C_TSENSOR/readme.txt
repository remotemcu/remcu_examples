/** @page I2C_TSENSOR Communication between I2C and STLM75 Temperature sensor

  @verbatim
  ******************** (C) COPYRIGHT 2013 STMicroelectronics *******************
  * @file    I2C/I2C_TSENSOR/readme.txt
  * @author  MCD Application Team
  * @version V1.5.2
  * @date    30-September-2014
  * @brief   Description of the I2C Temperature sensor Example.
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

  This example provides a description of how to use the I2C to communicate with 
  an STLM75 (or a compatible device   I2C temperature sensor that is mounted on
  the STM8L1528-EVAL and STM8L1526-EVAL boards and used to get instantaneous
  external temperature.
 
  The measured temperature is displayed on LCD dot matrix and the MCU enter in 
  active halt mode, after each 10s the RTC generates an interrupt to wake up the 
  MCU from halt mode. 


  @par Directory contents

  - I2C/I2C_TSENSOR/main.c            Main file containing the "main" function
  - I2C/I2C_TSENSOR/stm8l15x_conf.h   Library Configuration file
  - I2C/I2C_TSENSOR/stm8l15x_it.c     Interrupt routines source
  - I2C/I2C_TSENSOR/stm8l15x_it.h     Interrupt routines declaration
  - I2C/I2C_TSENSOR/interrupt.s       (to use only with IAR  


  @par Hardware and software environment

    - This example runs on STM8L15x High-Density, Medium-Density Plus, Medium-Density
    and Low-Density Devices.
  
  - This example has been tested with STMicroelectronics STM8L1528-EVAL 
    (STM8L15x High-Density devices) and STM8L1526-EVAL (STM8L15x Medium-Density and
    Low-Density devices) evaluation boards and can be easily tailored to any other
    development board.

  - STM8L1528-EVAL Set-up 
    - Make sure that the LCD glass daughter board is mounted in IO position.
       For more details please refer to the evaluation board user manual.
    - STLM75 temperature sensor is already mounted on STM8L1528-EVAL.

  - STM8L1526-EVAL Set-up
    - Make sure that the LCD glass daughter board is mounted in IO position.
       For more details please refer to the evaluation board user manual.
    - STLM75 temperature sensor is already mounted on STM8L1526-EVAL.
    - Make sure that Jumper JP7 is set in position 1-2 (I2C position  .

  @par How to use it ?

  In order to make the program work, you must do the following :

  - Copy all source files from this example folder to the template folder under
    Project\STM8L15x_StdPeriph_Template
  - Open your preferred toolchain 
  - Add the required files:
    - stm8_eval.c    (Under Utilities\STM8_EVAL)
    - stm8_eval_lcd.c    (Under Utilities\STM8_EVAL\Common)
    - stm8_eval_i2c_tsensor.c   (under Utilities\STM8_EVAL\Common)
  - Rebuild all files and load your image into target memory
  - Run the example
  
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
