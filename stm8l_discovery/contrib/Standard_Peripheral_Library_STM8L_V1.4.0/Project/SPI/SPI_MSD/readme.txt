/** @page SPI_MSD  Use the SPI and an associated driver to communicate with a MicroSD card

  @verbatim
  ******************** (C) COPYRIGHT 2013 STMicroelectronics *******************
  * @file    SPI/SPI_MSD/readme.txt 
  * @author  MCD Application Team
  * @version V1.5.2
  * @date    30-September-2014
  * @brief   Description of the SPI MSD Example.
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

  This example provides a short description of how to make a communication
  (Write & Read data) between the SPI and a MicroSD memory card like the
  one present on STM8L152X-EVAL boards.
  
  First, Fill the Tx_Buffer with data to be written into the MicroSD memory 
  followed by a read of that data.
  Then data read from the MicroSD memory stored in the Rx_Buffer are compared
  with the expected values of the Tx_Buffer. The result of this comparison is 
  stored in the "TransferStatus" variable.


 @par Directory contents

  - SPI/SPI_MSD/main.c            Main file containing the "main" function
  - SPI/SPI_MSD/stm8l15x_conf.h   Library Configuration file
  - SPI/SPI_MSD/stm8l15x_it.c     Interrupt routines source
  - SPI/SPI_MSD/stm8l15x_it.h     Interrupt routines declaration
    

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
     - LED1..4
     - Plug a MicroSD memory card into the dedicated connector (CN15).
    
  - STM8L1526-EVAL Set-up
     - Make sure that the LCD glass daughter board is mounted in IO position.
       For more details please refer to the evaluation board user manual.
     - LED1..4
     - Plug a MicroSD memory card into the dedicated connector (CN14).

  
  @par How to use it ?

  In order to make the program work, you must do the following :

  - Copy all source files from this example folder to the template folder under
    Project\STM8L15x_StdPeriph_Template
  - Open your preferred toolchain 
  - Add the required files:
    - stm8_eval.c   (under Utilities\STM8_EVAL)
    - stm8_eval_spi_sd.c (under Utilities\STM8_EVAL\Common)
  - Rebuild all files and load your image into target memory
  - Run the example
   - If the program is pass the four LEDs are toggled, otherwise only LED1 toggles.
  


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
