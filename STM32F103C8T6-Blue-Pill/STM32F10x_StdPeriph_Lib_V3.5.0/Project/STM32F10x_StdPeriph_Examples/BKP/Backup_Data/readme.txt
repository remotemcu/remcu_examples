/**
  @page BKP_Backup_Data Backup Data example
  
  @verbatim
  ******************** (C) COPYRIGHT 2011 STMicroelectronics *******************
  * @file    BKP/Backup_Data/readme.txt 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Description of the Backup Data example.
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

This example shows how to store user data in the Backup data registers.
As the Backup (BKP) domain still powered by VBAT when VDD is switched off, its
contents are not lost if a battery is connected to VBAT pin.

The program behaves as follows:

1. After startup the program checks if the board has been powered up. If yes, 
the values in the BKP data registers are checked:
   - if a battery is connected to the VBAT pin, the values in the BKP data registers
     are retained
   - if no battery is connected to the VBAT pin, the values in the BKP data registers
     are lost
     
2. After an external reset, the BKP data registers contents are not checked.

Four LEDs are used to monitor the system state as follows:
1. LD3 on / LD1 on: a Power On Reset occurred and the values in the BKP data
   registers are correct
   
2. LD3 on / LD2 on: a Power On Reset occurred and the values in the BKP data
   registers are not correct or they have not yet been programmed (if it is the
   first time the program is executed)
   
3. LD3 off / LD1 off / LD2 off: no Power On Reset occurred

4. LD4 on: program running

@par Directory contents 

  - BKP/Backup_Data/stm32f10x_conf.h     Library Configuration file
  - BKP/Backup_Data/stm32f10x_it.c       Interrupt handlers
  - BKP/Backup_Data/stm32f10x_it.h       Header for stm32f10x_it.c
  - BKP/Backup_Data/system_stm32f10x.c   STM32F10x system source file  
  - BKP/Backup_Data/main.c               Main program

@par Hardware and Software environment 

  - This example runs on STM32F10x Connectivity line, High-Density, Medium-Density, 
    XL-Density, High-Density Value line, Medium-Density Value line, Low-Density 
    and Low-Density Value line Devices.
  
  - This example has been tested with STMicroelectronics STM32100E-EVAL (High-Density 
    Value line), STM32100B-EVAL (Medium-Density Value line), STM3210C-EVAL 
    (Connectivity line), STM3210E-EVAL (High-Density and XL-Density) and 
    STM3210B-EVAL (Medium-Density) evaluation boards and can be easily tailored 
    to any other supported device and development board.
    To select the STMicroelectronics evaluation board used to run the example, 
    uncomment the corresponding line in stm32_eval.h file (under Utilities\STM32_EVAL)

  - STM32100B-EVAL Set-up  
    - Use LD1, LD2, LD3 and LD4 leds connected respectively to PC.06, PC.07, 
      PC.08 and PC.09 pins
    - Use 3V battery on VBAT pin (set jumper JP9 in position 1-2)  
    
  - STM3210C-EVAL Set-up 
    - Use LD1, LD2, LD3 and LD4 leds connected respectively to PD.07, PD.13, PF.03
      and PD.04 pins  
    - Use 3V battery on VBAT pin (set jumper JP24 in position 1-2)  

  - STM3210E-EVAL Set-up 
    - Use LD1, LD2, LD3 and LD4 leds connected respectively to PF.06, PF0.7, 
      PF.08 and PF.09 pins
    - Use 3V battery on VBAT pin (set jumper JP1 in position 1-2)  

  - STM3210B-EVAL Set-up  
    - Use LD1, LD2, LD3 and LD4 leds connected respectively to PC.06, PC.07, 
      PC.08 and PC.09 pins
    - Use 3V battery on VBAT pin (set jumper JP11 in position 1-2)  

  - STM32100E-EVAL Set-up 
    - Use LD1, LD2, LD3 and LD4 leds connected respectively to PF.06, PF0.7, 
      PF.08 and PF.09 pins
    - Use 3V battery on VBAT pin (set jumper JP1 in position 1-2) 
	
@par How to use it ? 

In order to make the program work, you must do the following :
 - Copy all source files from this example folder to the template folder under
   Project\STM32F10x_StdPeriph_Template
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example 
 - Power on/off the board and check that the BKP contents are not lost
   
@note
 - Low-density Value line devices are STM32F100xx microcontrollers where the 
   Flash memory density ranges between 16 and 32 Kbytes.
 - Low-density devices are STM32F101xx, STM32F102xx and STM32F103xx 
   microcontrollers where the Flash memory density ranges between 16 and 32 Kbytes.
 - Medium-density Value line devices are STM32F100xx microcontrollers where
   the Flash memory density ranges between 64 and 128 Kbytes.  
 - Medium-density devices are STM32F101xx, STM32F102xx and STM32F103xx 
   microcontrollers where the Flash memory density ranges between 64 and 128 Kbytes.
 - High-density Value line devices are STM32F100xx microcontrollers where
   the Flash memory density ranges between 256 and 512 Kbytes.
 - High-density devices are STM32F101xx and STM32F103xx microcontrollers where
   the Flash memory density ranges between 256 and 512 Kbytes.
 - XL-density devices are STM32F101xx and STM32F103xx microcontrollers where
   the Flash memory density ranges between 512 and 1024 Kbytes.
 - Connectivity line devices are STM32F105xx and STM32F107xx microcontrollers.
   
 * <h3><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h3>
 */
