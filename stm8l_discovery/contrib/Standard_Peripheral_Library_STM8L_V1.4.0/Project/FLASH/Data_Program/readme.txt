/** @page Data_Program How to use the block operations to Write/Erase the Data Memory

  @verbatim
  ******************** (C) COPYRIGHT 2013 STMicroelectronics *******************
  * @file    FLASH/Data_Program/readme.txt
  * @author  MCD Application Team
  * @version V1.5.2
  * @date    30-September-2014
  * @brief   Description of how to program the STM8L15x FLASH Data Memory using 
  *          the Write/erase block operations.
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

  This example provides a description of how to use FLASH Write/Erase Block mode 
  on data eeprom memory.
  After Reset, the Flash memory Program/Erase Controller is locked. 
  To unlock it, the FLASH_Unlock function is used. Once done, the following 
  operations on the  data memory will be performed:
   - Program a block at a specified address (0x1000) by using FLASH_ProgramBlock()
     function.
   - Check program block (Read byte) if the program block operation failed LED1 
     will toggle.
   - Erase block using FLASH_EraseBlock() function.
   - Check erase (using Read byte) if the program block operation failed LED2 
     will toggle.
  - If Write/Erase operations passed LED3 will toggle. 
  
  @note Within the main.c file, you will find details on the procedures to follow 
        to execute FLASH_ProgramBlock() and FLASH_EraseBlock() functions from RAM. 


  @par Directory contents

  - FLASH/Data_Program/main.c               Main file
  - FLASH/Data_Program/stm8l15x_conf.h      Library Configuration file
  - FLASH/Data_Program/stm8l15x_it.c        Interrupt routines source 
  - FLASH/Data_Program/stm8l15x_it.h        Interrupt routines declaration


  @par Hardware and Software environment

    - This example runs on STM8L15x High-Density, Medium-Density Plus, Medium-Density
    and Low-Density Devices.
  
  - This example has been tested with STMicroelectronics STM8L1528-EVAL 
    (STM8L15x High-Density devices) and STM8L1526-EVAL (STM8L15x Medium-Density and
    Low-Density devices) evaluation boards and can be easily tailored to any other
    development board.

  - STM8L1528-EVAL and STM8L1526-EVAL Set-up
     - Make sure that the LCD glass daughter board is mounted in IO position.
      For more details please refer to the evaluation board user manual.
     - LD1..4


  @par How to use it ?

  In order to make the program work, you must do the following :

  - Copy all source files from this example folder to the template folder under
    Project\STM8L15x_StdPeriph_Template
  - Open your preferred toolchain
  - Add the required file:
    - stm8_eval.c   (under Utilities\STM8_EVAL) 
  - For Cosmic Compiler
       - In STVD Select Project\Settings\Linker\Category "input" and in the 
         RAM section add the FLASH_CODE segment with "-ic" options.
       - Uncomment the line "#define RAM_EXECUTION (1)" in the stm8l15x.h 
         file to enable the FLASH functions execution from RAM.

  - For Raisonance Compiler
       - Uncomment the line "#define RAM_EXECUTION (1)" in the 
         stm8l15x.h file to enable the FLASH functions execution from RAM through the 
         specific inram directive.

  - For IAR Compiler
       - Uncomment the line "#define RAM_EXECUTION (1)" in the 
         stm8l15x.h file to enable the FLASH functions execution from RAM through 
         the specific __ramfunc keyword.
       
  - Rebuild all files and load your image into target memory
  - Run the example




 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */