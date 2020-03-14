/**
  @page CAN_DualCAN Dual CAN example
  
  @verbatim
  ******************** (C) COPYRIGHT 2011 STMicroelectronics *******************
  * @file    CAN/DualCAN/readme.txt 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Description of the Dual CAN example.
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

This example shows how to configure the CAN1 and CAN2 peripherals to send and 
receive CAN frames in normal mode. The sent frames are used to control Leds by 
pressing KEY or TAMPER push buttons:
 - When KEY button is pressed, LED1 is turned ON and CAN1 sends a message to CAN2,
   when CAN2 receives correctly this message LED4 is turned ON. 
 - When TAMPER button is pressed, LED2 is turned ON and CAN2 sends a message to CAN1,
   when CAN1 receives correctly this message LED3 is turned ON. 

The CAN1 and CAN2 are configured as follow:
    - Bit Rate   = 1 Mbit/s  
    - CAN Clock  = external clock (HSE)
    - ID Filter  = All identifiers are allowed
    - RTR = Data
    - DLC = 1 byte
    - Data: Led number that should be turned ON
@note 
  user can select one from the preconfigured CAN baud rates from the private 
  defines in main.c by uncommenting the desired define: 

@code
  #define CAN_BAUDRATE  1000      /* CAN baudrate = 1MBps   */
/* #define CAN_BAUDRATE  500*/  /* CAN baudrate = 500kBps */
/* #define CAN_BAUDRATE  250*/  /* CAN baudrate = 250kBps */
/* #define CAN_BAUDRATE  125*/  /* CAN baudrate = 125kBps */
/* #define CAN_BAUDRATE  100*/  /* CAN baudrate = 100kBps */ 
/* #define CAN_BAUDRATE  50*/   /* CAN baudrate = 50kBps  */ 
/* #define CAN_BAUDRATE  20*/   /* CAN baudrate = 20kBps  */ 
/* #define CAN_BAUDRATE  10*/   /* CAN baudrate = 10kBps  */
@endcode

@par Directory contents 

  - CAN/DualCAN/stm32f10x_conf.h     Library Configuration file
  - CAN/DualCAN/stm32f10x_it.c       Interrupt handlers
  - CAN/DualCAN/stm32f10x_it.h       Interrupt handlers header file
  - CAN/DualCAN/main.c               Main program
  - CAN/DualCAN/system_stm32f10x.c   STM32F10x system source file

@par Hardware and Software environment 

  - This example runs on STM32F10x Connectivity line Devices.
  
  - This example has been tested with STMicroelectronics STM3210C-EVAL (STM32F10x 
    Connectivity line) evaluation board and can be easily tailored to any other 
    supported device and development board.
    To select the STMicroelectronics evaluation board used to run the example, 
    uncomment the corresponding line in stm32_eval.h file  

  - STM3210C-EVAL Set-up 
    - Use LED1, LED2, LED3 and LED4 connected respectively to PD.07, PD.13, PF.03
      and PD.04 pins
    - Use Key Push Button connected to PB9
    - Use Tamper Push Button connected to PC13    
    - Connect a female/female CAN cable between the CAN connectors 
      (CN4 and CN3 on STM3210C-EVAL boards)
    - Connector 1 DB9_PIN2 to Connector 2 DB9_PIN2  (CAN_L)
    - Connector 1 DB9_PIN5 to Connector 2 DB9_PIN5  ( GND )
    - Connector 1 DB9_PIN7 to Connector 2 DB9_PIN7  (CAN_H) 
    - JP6 or JP5 must be fitted.

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
