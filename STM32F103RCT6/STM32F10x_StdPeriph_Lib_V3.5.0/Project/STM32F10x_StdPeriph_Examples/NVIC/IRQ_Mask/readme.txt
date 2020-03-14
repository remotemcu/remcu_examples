/**
  @page NVIC_IRQ_Mask NVIC IRQ Mask example
  
  @verbatim
  ******************** (C) COPYRIGHT 2011 STMicroelectronics *******************
  * @file    NVIC/IRQ_Mask/readme.txt 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Description of the NVIC IRQ Mask example.
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

This example demontrates the use of the Nested Vectored Interrupt Controller (NVIC)
IRQ Channels configuration and how to mask/activate different IRQs: 

- Configuration of 3 TIM (TIM2..TIM4)timers to generate an interrupt on each 
  counter update event.

- The three timers are linked to their correspondant Update IRQ channel.

- Assignment of a ascendant IRQ priority for each IRQ channel :
     - TIM2 has a preemption priority of 0
     - TIM3 has a preemption priority of 1
     - TIM4 has a preemption priority of 2
- In each interrupt routine: 
   - TIM2 toggles a LED1 each 1s 
   - TIM3 toggles a LED2 each 2s 
   - TIM4 toggles a LED3 each 3s 

- The KEY and WAKEUP buttons are used to boost the execution priority as follows:

  - The KEY button is used in GPIO mode and at each KEY button press, the execution 
    priority is raised to 0 and turn LED4 ON. This prevents all exceptions with 
    configurable priority from activating, other than through the HardFault fault 
    escalation mechanism. As consequence, all LEDs stop toggling as TIM2, TIM3 
    and TIM4 IRQs are prevented from activation. 
    
    Pressing again the KEY button will release the priority boosting, turn LED4 
    OFF and will allow all exceptions with configurable priority to be activated 
    and TIM2, TIM3 and TIM4 can be generated again and the LEDs restart toggling.
    
    This execution priority is made using the CMSIS functions "__disable_irq()" 
    and "__enable_irq()". 
    These two functions are managing the Cortex-M3 PRIMASK special register.

  - The WAKEUP button is used in EXTI mode and at each WAKEUP button press, the execution 
    priority is masked to 0x40 using the BASEPRI register. 
    A non-zero value will act as a priority mask, affecting the execution priority 
    when the priority defined by BASEPRI is the same or higher than the current 
    executing priority. 
    As consequence, LED2 and LED3 stop toggling as TIM3 and TIM4 IRQs are 
    prevented from activation. 
    Pressing again the WAKEUP button will configure the BASEPRI register to 0, 
    thus it has no effect on the current priority and TIM3 and TIM4 can be 
    generated again and LED2 and LED3 restart toggling.
    
    This execution priority is made using the CMSIS functions "__set_BASEPRI()". 
    This function is managing the Cortex-M3 BASEPRI special register.
    Setting the BASEPRI register has no effect when the execution priority is
    raised to 0 using the "__disable_irq()" function.

@note These mechanisms only affect the group priority. They have no effect on 
      the sub-priority. The sub-priority is only used to sort pending exception 
      priorities, and does not affect active exceptions.

@par Directory contents 

  - NVIC/IRQ_Mask/stm32f10x_conf.h    Library Configuration file
  - NVIC/IRQ_Mask/stm32f10x_it.c      Interrupt handlers
  - NVIC/IRQ_Mask/stm32f10x_it.h      Interrupt handlers header file
  - NVIC/IRQ_Mask/main.c              Main program
  - NVIC/IRQ_Mask/system_stm32f10x.c  STM32F10x system source file
  
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
    - Use LED1, LED2, LED3 and LED4 leds connected respectively to PF.06, PF.07
      PF.08 and PF.09
    - Use the Key push-button connected to pin PG.08 (EXTI Line8). 
    - Use the Wakeup push-button connected to pin PA.00 (EXTI Line0). 
    @note  the jumper JP4 must be not fit to be able to use the Wakeup push-button    
            
  - STM32100B-EVAL Set-up  
    - Use LED1, LED2, LED3 and LED4 leds connected respectively to PC.06, PC.07,
      PC.08 and PC.09
    - Use the Key push-button connected to pin PB.09 (EXTI Line9).
    - Use the Wakeup push-button connected to pin PA.00 (EXTI Line0).    
        
  - STM3210C-EVAL Set-up 
    - Use LED1, LED2, LED3 and LED4 connected respectively to PD.07, PD.13, PF.03
      and PD.04 pins
    - Use the Key push-button connected to pin PB.09 (EXTI Line9).
    - Use the Wakeup push-button connected to pin PA.00 (EXTI Line0).         
    @note  Make sure that the Jumper 14 is in position 2<->3.
              
  - STM3210E-EVAL Set-up 
    - Use LED1, LED2, LED3 and LED4 leds connected respectively to PF.06, PF.07
      PF.08 and PF.09
    - Use the Key push-button connected to pin PG.08 (EXTI Line8).   
    - Use the Wakeup push-button connected to pin PA.00 (EXTI Line0).
    @note  the jumper JP4 must be not fit to be able to use the Wakeup push-button          

  - STM3210B-EVAL Set-up  
    - Use LED1, LED2, LED3 and LED4 leds connected respectively to PC.06, PC.07,
      PC.08 and PC.09
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
