/** @page TIM1_DMABurstMode Use DMA to transfer data in Burst mode from RAM buffer to TIM1

  @verbatim
  ******************** (C) COPYRIGHT 2013 STMicroelectronics *******************
  * @file    TIM1/TIM1_DMABurstMode/readme.txt 
  * @author  MCD Application Team
  * @version V1.5.2
  * @date    30-September-2014
  * @brief   Description of the TIM1 DMA burst mode Example.
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


  @par Firmware description

  This application provides a short description of how to use the DMA peripheral 
  to transfer data in Burst mode from RAM Buffer to TIM1.
  TIM1 channel 1 is configured in PWM output mode, DMA channel 2 is configured 
  to transfer data from RAM memory to TIM1 registers (PSCRH, PSCRL, ARRH, ARRL,
  RCR, CCR1H, CCR1L  .
  The goal of this example is to update simultaneously, on an external event
  provided from Key Button, the frequency and duty cycle TIM1 channel1.
  When starting the application, read the values of TIM1 registers: 
   PSCRH, PSCRL, ARRH, ARRL, RCR, CCR1H, CCR1L.

  After the 1st key press, and any odd key press, TIM1 registers values will be 
  as follow:
    - PSCRH = 0xFF
    - PSCRL = 0xEE
    - ARRH = 0xDD
    - ARRL = 0xCC
    - RCR = 0xBB
    - CCR1H = 0xAA
    - CCR1L = 0x99
  
  After the 2nd key press, and any even key press, TIM1 registers values will be 
  as follow: 
    - PSCRH = 0x77
    - PSCRL = 0x66
    - ARRH = 0x55
    - ARRL = 0x44
    - RCR = 0x33
    - CCR1H = 0x22
    - CCR1L = 0x11


  @par Directory contents

  - TIM1/TIM1_DMABurstMode/main.c             Main file containing the "main" function
  - TIM1/TIM1_DMABurstMode/stm8l15x_conf.h    Library Configuration file
  - TIM1/TIM1_DMABurstMode/stm8l15x_it.c      Interrupt routines source
  - TIM1/TIM1_DMABurstMode/stm8l15x_it.h      Interrupt routines declaration


 @par Hardware and Software environment

    - This example runs on STM8L15x High-Density, Medium-Density Plus and Medium-Density
    Devices.

  - This example has been tested with STMicroelectronics STM8L1528-EVAL 
    (STM8L15x High-Density devices) and STM8L1526-EVAL (STM8L15x Medium-Density 
    devices) evaluation boards and can be easily tailored to any other
    development board.

  - STM8L1528-EVAL Set-up
    - Make sure that the LCD glass daughter board is mounted in IO position.
      For more details please refer to the evaluation board user manual.
    - KEY push-buttons.
  
  - STM8L1526-EVAL Set-up
    - Make sure that the LCD glass daughter board is mounted in IO position.
      For more details please refer to the evaluation board user manual.
    - KEY push-buttons.
    - Make sure that jumper JP7 is in key position.
  
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