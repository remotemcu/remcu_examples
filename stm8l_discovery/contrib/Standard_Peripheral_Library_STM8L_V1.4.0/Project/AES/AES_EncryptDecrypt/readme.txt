/** @page AES_EncryptDecrypt  Use AES to encrypt then to decrypt data

  @verbatim
  ******************** (C) COPYRIGHT 2013 STMicroelectronics *******************
  * @file    AES/AES_EncryptDecrypt/readme.txt 
  * @author  MCD Application Team
  * @version V1.5.2
  * @date    30-September-2014
  * @brief   Description of the AES encryption decryption example.
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

  This example provides a short description of how to configure the AES hardware 
  accelerator to encrypt then to decrypt text and to check that decrypted text 
  corresponds to the original text.
  This example is a basic example using polling on flag, neither interrupt 
  nor DMA capability are used.
  
  @par Directory contents

  - AES/AES_EncryptDecrypt/main.c                   Main file
  - AES/AES_EncryptDecrypt/stm8l15x_conf.h          Library Configuration file
  - AES/AES_EncryptDecrypt/stm8l15x_it.c            Interrupt routines source
  - AES/AES_EncryptDecrypt/stm8l15x_it.h            Interrupt routines declaration


  @par Hardware and Software environment

  - This example runs on STM8L15x High-Density Devices with built-in AES peripheral.
  
  - This example has been tested with STMicroelectronics STM8L1528-EVAL evaluation
    board and can be easily tailored to any other development board.

  - STM8L1528-EVAL Set-up
     - Make sure that the LCD glass daughter board is mounted in IO position.
       For more details please refer to the evaluation board user manual.
     - LED1 and LED3


  @par How to use it ?

  In order to make the program work, you must do the following :

  - Copy all source files from this example folder to the template folder under
    Project\STM8L15x_StdPeriph_Template
  - Open your preferred toolchain
  - Choose STM8L15X_HD(STM8L15xx8_16xx8) project 
   - Add the required file : 
     - stm8_eval.c   (under Utilities\STM8_EVAL)
  - Rebuild all files and load your image into target memory
  - Run the example
  - If green led LED1 is ON, text is successfully encrypted then decrypted.
  - If red led LED3 is ON, an error occurs while encrypting or decrypting.
      


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */