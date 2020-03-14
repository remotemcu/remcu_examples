/**
  @page EXTI_Config EXTI configuration example
  
  @verbatim
  ******************** (C) COPYRIGHT 2011 STMicroelectronics *******************
  * @file    EXTI/EXTI_Config/readme.txt 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Description of the EXTI configuration example.
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

This example shows how to configure external interrupt lines.
In this example, 2 EXTI lines (EXTI0 and EXTI8 or EXTI9) are configured 
to generate an interrupt on each rising or falling edge. In the interrupt routine 
a led connected to a specific GPIO pin is toggled.

In this example:
  - EXTI0 is mapped to PA.00
  - EXTI8 is mapped to PG.08
  - EXTI9 is mapped to PB.09

After EXTI configuration, a software interrupt is generated on the EXTI0 toggles LED1.
After that,
  when rising edge is detected on EXTI0, LED1 toggles
  when falling edge is detected on EXTI8, LED2 toggles
  when falling edge is detected on EXTI9, LED2 toggles


If any STM3210XX-EVAL is used,
  when Wakeup push-button is pressed, LED1 toggles
  when Key push-button is pressed, LED2 toggles

In this example, EXTI0_Config() function can be substituted by 
STM_EVAL_PBInit(Button_WAKEUP, Mode_EXTI) and EXTI9_5_Config() by 
STM_EVAL_PBInit(Button_KEY, Mode_EXTI) provided in the STM3210XX-EVAL driver.


@par Directory contents 

  - EXTI/EXTI_Config/stm32f10x_conf.h    Library Configuration file
  - EXTI/EXTI_Config/stm32f10x_it.h      Interrupt handlers header file
  - EXTI/EXTI_Config/stm32f10x_it.c      Interrupt handlers
  - EXTI/EXTI_Config/main.c              Main program
  - EXTI/EXTI_Config/system_stm32f10x.c  STM32F10x system source file
  
@par Hardware and Software environment 

  - This example runs on STM32F10x Connectivity line, High-Density, High-Density
    Value line, Medium-Density, XL-Density, Medium-Density Value line, Low-Density 
    and Low-Density Value line Devices.
  
  - This example has been tested with STMicroelectronics STM32100E-EVAL (High-Density
    Value line), STM32100B-EVAL (Medium-Density Value line), STM3210C-EVAL 
    (Connectivity line), STM3210E-EVAL (High-Density and XL-Density) and STM3210B-EVAL 
    (Medium-Density) evaluation boards and can be easily tailored to any other 
    supported device and development board.
    To select the STMicroelectronics evaluation board used to run the example, 
    uncomment the corresponding line in stm32_eval.h file (under Utilities\STM32_EVAL)

  - STM32100E-EVAL Set-up 
    - Use LED1 and LED2 connected to PF.06 and PF.07 pin
    - Use the Key push-button connected to pin PG.08 (EXTI Line8). 
    - Use the Wakeup push-button connected to pin PA.00 (EXTI Line0).
    @note  the jumper JP4 must be not fit to be able to use the Wakeup push-button
        
  - STM32100B-EVAL Set-up 
    - Use LED1 and LED2 connected to PC.06 and PC.07 pin
    - Use the Key push-button connected to pin PB.09 (EXTI Line9). 
    - Use the Wakeup push-button connected to pin PA.00 (EXTI Line0).
        
  - STM3210C-EVAL Set-up 
    - Use LED1 and LED2 connected to PD.07 and PD.13 pin
    - Use the Key push-button connected to pin PB.09 (EXTI Line9).
    - Use the Wakeup push-button connected to pin PA.00 (EXTI Line0). Make sure 
      that the Jumper 14 is in position 2<->3.
      
  - STM3210E-EVAL Set-up 
    - Use LED1 and LED2 connected to PF.06 and PF.07 pin
    - Use the Key push-button connected to pin PG.08 (EXTI Line8). 
    - Use the Wakeup push-button connected to pin PA.00 (EXTI Line0).
    @note  the jumper JP4 must be not fit to be able to use the Wakeup push-button
    
  - STM3210B-EVAL Set-up 
    - Use LED1 and LED2 connected to PC.06 and PC.07 pin
    - Use the Key push-button connected to pin PB.09 (EXTI Line9).     
    - Use the Wakeup push-button connected to pin PA.00 (EXTI Line0).
    
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
