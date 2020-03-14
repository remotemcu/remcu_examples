/**
  @page NVIC_IRQ_Priority NVIC IRQ Priority example
  
  @verbatim
  ******************** (C) COPYRIGHT 2011 STMicroelectronics *******************
  * @file    NVIC/IRQ_Priority/readme.txt 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Description of the NVIC IRQ Priority example.
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

This example demonstrates the use of the Nested Vectored Interrupt Controller (NVIC): 

- Configuration of 2 EXTI Lines (Wakeup button EXTI Line & Key button EXTI Line)
  to generate an interrupt on each falling edge and use the SysTick interrupt.
- These interrupts are configured with the following parameters:
    - Wakeup button EXTI Line:  
        - PreemptionPriority = PreemptionPriorityValue
        - SubPriority = 0
    - Key button EXTI Line:    
        - PreemptionPriority = 0
        - SubPriority = 1           
    - SysTick Handler:  
        - PreemptionPriority = !PreemptionPriorityValue
        - SubPriority = 0             
First, the PreemptionPriorityValue is equal to 0, the Wakeup button EXTI Line 
has higher preemption priority than the SysTick handler. 

In the key button EXTI Line interrupt routine the Wakeup button EXTI Line and 
SysTick preemption priorities are inverted. 
In the Wakeup button EXTI Line interrupt routine the pending bit of the SysTick 
interrupt is set this will cause SysTick ISR to preempt the Wakeup button EXTI 
Line ISR only if it has higher preemption priority.

The system behaves as following:
 
1) The first time Key button EXTI Line interrupt occurs the SysTick preemption 
become higher than Wakeup button EXTI Line one. So when the Wakeup button EXTI 
Line interrupt occurs, the SysTick ISR is executed and the PreemptionOccured 
variable become TRUE and the four leds (LED1, LED2, LED3, LED4) start toggling.

2) When the next Key button EXTI Line interrupt occurs the SysTick preemption
become lower than Wakeup button EXTI Line one. So when the Wakeup button EXTI Line
interrupt occurs, the PreemptionOccured variable became FALSE and the four leds
(LED1, LED2, LED3, LED4) stop toggling.

Then this behavior is repeated from point 1) in an infinite loop.


@par Directory contents 

  - NVIC/IRQ_Priority/stm32f10x_conf.h     Library Configuration file
  - NVIC/IRQ_Priority/stm32f10x_it.c       Interrupt handlers
  - NVIC/IRQ_Priority/stm32f10x_it.h       Interrupt handlers header file
  - NVIC/IRQ_Priority/main.c               Main program
  - NVIC/IRQ_Priority/system_stm32f10x.c   STM32F10x system source file

@par Hardware and Software environment 

  - This example runs on STM32F10x Connectivity line, High-Density, High-Density 
    Value line, Medium-Density, XL-Density, Medium-Density Value line, Low-Density 
    and Low-Density Value line Devices.
  
  - This example has been tested with STMicroelectronics STM32100E-EVAL (High-Density 
    Value line), STM32100B-EVAL (Medium-Density Value line), STM3210C-EVAL (Connectivity line), 
    STM3210E-EVAL (High-Density and XL-Density) and STM3210B-EVAL (Medium-Density)
    evaluation boards and can be easily tailored to any other supported device 
    and development board.
    To select the STMicroelectronics evaluation board used to run the example, 
    uncomment the corresponding line in stm32_eval.h file (under Utilities\STM32_EVAL)

  - STM32100E-EVAL Set-up 
    - Use LED1, LED2, LED3 and LED4 connected respectively to PF.06, PF0.7, PF.08 
      and PF.09 pins
    - Use the Key push-button connected to pin PG.08 (EXTI Line8).
    - Use the Wakeup push-button connected to pin PA.00 (EXTI Line0).
    @note  the jumper JP4 must be not fit to be able to use the Wakeup push-button
        
  - STM32100B-EVAL Set-up  
    - Use LED1, LED2, LED3 and LED4 connected respectively to PC.06, PC.07, PC.08
      and PC.09 pins
    - Use the Key push-button connected to pin PB.09 (EXTI Line9).
    - Use the Wakeup push-button connected to pin PA.00 (EXTI Line0).  
    
 - STM3210C-EVAL Set-up  
    - Use LED1, LED2, LED3 and LED4 connected respectively to PD.07, PD.13, PF.03
      and PD.04 pins
    - Use the Key push-button connected to pin PB.09 (EXTI Line9).
    - Use the Wakeup push-button connected to pin PA.00 (EXTI Line0). Make sure 
      that the Jumper 14 is in position 2<->3.
      
  - STM3210E-EVAL Set-up 
    - Use LED1, LED2, LED3 and LED4 connected respectively to PF.06, PF0.7, PF.08 
      and PF.09 pins
    - Use the Key push-button connected to pin PG.08 (EXTI Line8).
    - Use the Wakeup push-button connected to pin PA.00 (EXTI Line0).
    @note  the jumper JP4 must be not fit to be able to use the Wakeup push-button
    
  - STM3210B-EVAL Set-up  
    - Use LED1, LED2, LED3 and LED4 connected respectively to PC.06, PC.07, PC.08
      and PC.09 pins
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
