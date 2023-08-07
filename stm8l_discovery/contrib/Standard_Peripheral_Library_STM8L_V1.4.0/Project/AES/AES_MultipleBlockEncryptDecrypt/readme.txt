/** @page AES_MultipleBlockEncryptDecrypt  Use AES to encrypt then to decrypt data greater than 16 bytes (128) bits  

  @verbatim
  ******************** (C) COPYRIGHT 2013 STMicroelectronics *******************
  * @file    AES/AES_MultipleBlockEncryptDecrypt/readme.txt
  * @author  MCD Application Team
  * @version V1.5.2
  * @date    30-September-2014
  * @brief   Description of the AES encryption decryption multiple blocks example.
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
  PlainText[] buffer contains the plain text (original data   to be encrypted.
  CypherText[] buffer contains the encrypted plain text.
  ComputedPlainText[] buffer contains the decrypted cypher text.

              -------------                 --------------
 PlainText[]  |           |  CypherText[]   |            | ComputedPlainText[]
 -----------> | Encryption|---------------> | Decryption |--------------------->
              |           |                 |            |
              -------------                 --------------

  If no error occurs, ComputedPlainText[] should correspond to PlainText[].

  In this example, DMA is used to transfer data from/to memory to/from AES data register:

    - DMA1 Channel 0 is used during input phase: DMA transfer request is
    generated each time AES requires a byte to be written in AES_DINR register
    to transfer data from memory to AES_DINR register.

    - DMA1 Channel 3 is used during output phase: DMA transfer request is
    generated each time AES requires a byte to be read from AES_DOUTR register to
    transfer data from AES_DOUTR register to memory.

  This example is split into three parts:
     1- Encrypting data: AES peripheral is configured in encryption operation mode
     2- Decrypting data: AES peripheral is configured in decryption operation mode
     3- Checking buffers

  @Note: PlainText[] size must be multiple of 16 bytes since AES peripheral
         is a 16 bytes (128-bit   hardware accelerator.

  @Note: PlainText[] size must be lower than 112 bytes.

  @Note: The constraint on maximum data length (112 bytes   is due the maximum
  buffer size that can be transferred by DMA which is 255 bytes of data
  alternating plain text and encryption key.
  So plain text maximum size is 127 bytes but the PlainText[] size must be
  multiple of 16 bytes. Thus, in this example, maximum data length is 112 bytes.
  If more than 112 bytes are required the example must be updated.

  @par Directory contents

  - AES/AES_MultipleBlockEncryptDecrypt/main.c            Main file
  - AES/AES_MultipleBlockEncryptDecrypt/main.h            Main header
  - AES/AES_MultipleBlockEncryptDecrypt/stm8l15x_conf.h   Library Configuration file
  - AES/AES_MultipleBlockEncryptDecrypt/stm8l15x_it.c     Interrupt routines source
  - AES/AES_MultipleBlockEncryptDecrypt/stm8l15x_it.h     Interrupt routines declaration


  @par Hardware and Software environment

  - This example runs on STM8L15x High-Density Devices with built-in AES peripheral.

  - This example has been tested with STMicroelectronics STM8L1528-EVAL
  evaluation board and can be easily tailored to any other development board.

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
  - If green led LED1 is ON, PlainText[] matches ComputedPlainText[].
  - If red led LED3 is ON, PlainText[] doesn't match ComputedPlainText[].



 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */