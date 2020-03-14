/**
  @page BKP_Tamper BKP Tamper example
  
  @verbatim
  ******************** (C) COPYRIGHT 2011 STMicroelectronics *******************
  * @file    BKP/Tamper/readme.txt 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Description of the BKP Tamper example.
  ******************************************************************************
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  ******************************************************************************
   @endverbatim

@par Example Description 

This example shows how to write/read data to/from Backup data registers and 
demonstrates the Tamper detection feature.

The associated firmware performs the following:

1. It configures the ANTI_TAMP pin to be active low, and enables the Tamper interrupt.

2. It writes the data to all Backup data registers, then check whether the data were
correctly written. If yes, LED1 turns on, otherwise LED2 turns on.

3. On applying a low level on the ANTI_TAMP pin (PC.13), the Backup data registers
are reset and the Tamper interrupt is generated. The corresponding ISR then checks
whether the Backup data registers are cleared. If yes, LED3 on, otherwise LED4
turns on.


@par Directory contents 

  - BKP/Tamper/stm32f10x_conf.h    Library Configuration file
  - BKP/Tamper/stm32f10x_it.h      Interrupt handlers header file
  - BKP/Tamper/stm32f10x_it.c      Interrupt handlers
  - BKP/Tamper/main.h              Main header file
  - BKP/Tamper/main.c              Main program
  - BKP/Tamper/system_stm32f10x.c  STM32F10x system source file


@par Hardware and Software environment 

  - This example runs on STM32F10x Connectivity line, High-Density, Medium-Density, 
    High-Density Value line, XL-Density, Medium-Density Value line, Low-Density 
    and Low-Density Value line Devices.
  
  - This example has been tested with STMicroelectronics STM32100E-EVAL (High-Density 
    Value line), STM32100B-EVAL (Medium-Density Value line), STM3210C-EVAL 
    (Connectivity line), STM3210E-EVAL (High-Density and XL-Density) and STM3210B-EVAL 
    (Medium-Density) evaluation boards and can be easily tailored to any other 
    supported device and development board.
    To select the STMicroelectronics evaluation board used to run the example, 
    uncomment the corresponding line in stm32_eval.h file (under Utilities\STM32_EVAL)

  - STM32100B-EVAL Set-up  
    - Use LD1, LD2, LD3 and LD4 leds connected respectively to PC.06, PC.07, 
      PC.08 and PC.09 pins
    - Use the Tamper push-button connected to pin PC.13. PC13 is already 
      connected to VDD on the eval board.
    
  - STM3210C-EVAL Set-up 
    - Use LD1, LD2, LD3 and LD4 leds connected respectively to PD.07, PD.13, PF.03
      and PD.04 pins  
    - Use the Tamper push-button connected to pin PC.13 (set jumper JP1 in position 2-3). 
      PC13 is already connected to VDD on the eval board.
    
  - STM3210E-EVAL Set-up 
    - Use LD1, LD2, LD3 and LD4 leds connected respectively to PF.06, PF0.7, 
      PF.08 and PF.09 pins
    - Use the Tamper push-button connected to pin PC.13. PC13 is already 
      connected to VDD on the eval board.   

  - STM3210B-EVAL Set-up  
    - Use LD1, LD2, LD3 and LD4 leds connected respectively to PC.06, PC.07, 
      PC.08 and PC.09 pins
    - Use the Tamper push-button connected to pin PC.13. PC13 is already 
      connected to VDD on the eval board.
	  
  - STM32100E-EVAL Set-up 
    - Use LD1, LD2, LD3 and LD4 leds connected respectively to PF.06, PF0.7, 
      PF.08 and PF.09 pins
    - Use the Tamper push-button connected to pin PC.13. PC13 is already 
      connected to VDD on the eval board.   
         
@par How to use it ? 

In order to make the program work, you must do the following :
 - Copy all source files from this example folder to the template folder under
   Project\STM32F10x_StdPeriph_Template
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example 
   
@note
 - Low-density Value line devices are STM32F100xx microcontrollers where the 
   Flash memory density ranges between 16 and 32 Kbytes.
 - Low-density devices are STM32F101xx, STM32F102xx and STM32F103xx 
   microcontrollers where the Flash memory density ranges between 16 and 32 Kbytes.
 - Medium-density Value line devices are STM32F100xx microcontrollers where
   the Flash memory density ranges between 64 and 128 Kbytes.  
 - Medium-density devices are STM32F101xx, STM32F102xx and STM32F103xx 
   microcontrollers where the Flash memory density ranges between 64 and 128 Kbytes
 - High-density Value line devices are STM32F100xx microcontrollers where
   the Flash memory density ranges between 256 and 512 Kbytes.
 - High-density devices are STM32F101xx and STM32F103xx microcontrollers where
   the Flash memory density ranges between 256 and 512 Kbytes.
 - XL-density devices are STM32F101xx and STM32F103xx microcontrollers where
   the Flash memory density ranges between 512 and 1024 Kbytes.
 - Connectivity line devices are STM32F105xx and STM32F107xx microcontrollers.
   
 * <h3><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h3>
 */
