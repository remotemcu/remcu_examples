/**
  @page Demo_Binary   Description of the STM32F4-Discovery Demonstration firmware's binary files

  @verbatim
  ******************** (C) COPYRIGHT 2011 STMicroelectronics *******************
  * @file    readme.txt 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    19-September-2011
  * @brief   Description of the STM32F4-Discovery Demonstration firmware's binary files.
  ******************************************************************************
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  ******************************************************************************
   @endverbatim


@par Description

This directory contains binary files (*.hex and *.dfu) of the STM32F4-Discovery 
Demonstration firmware. 
You can use the STM32F4xx’s embedded Bootloader or any in-system programming tool
to reprogram the demonstration using these binary files, as described below:

 + Using "in-system programming tool"
    - Connect the STM32F4-Discovery board to a PC with a 'USB type A to Mini-B'
      cable through USB connector CN1 to power the board.
    - Make sure that the embedded ST-LINK/V2 is configured for in-system programming
      (both CN3 jumpers ON)
    - Use "STM32F4-Discovery_Demonstration_V1.0.0.hex" binary with your preferred 
      in-system programming tool to reprogram the demonstration firmware 
      (ex. STM32 ST-LINK Utility, available for download from www.st.com).
 
 + Using "Bootloader (USB FS Device in DFU mode)"
    - Configure the STM32F4-Discovery board to boot from "System Memory" (boot pins BOOT0:1 / BOOT1:0)
        - Set BOOT0 pin to high level: on the male header P2 place a jumper between
          BOOT0 pin and VDD pin 
        - Set BOOT1(PB2) pin to low level: on the male header P1 place a jumper
          between PB2 pin and GND pin        
    - Connect a 'USB type A to Mini-B' cable between PC and USB connector CN1
      to power the board.
    - Connect a 'USB type A to Micro-B' cable between PC and USB connector CN5,
      the board will be detected as USB device.
    - Use "STM32F4-Discovery_Demonstration_V1.0.0.dfu" binary with "DFUse\DFUse Demonstration"
      tool (available for download from www.st.com) to reprogram the demonstration firmware 
 
 * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
 */
