/**
  @page SDIO_uSDCard SDIO Micro SD Card example
  
  @verbatim
  ******************** (C) COPYRIGHT 2011 STMicroelectronics *******************
  * @file    SDIO/uSDCard/readme.txt 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Description of the SDIO Micro SD Card example.
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

This example provides a basic example of how to use the SDIO firmware library and
an associate driver to perform read/write operations on the SD Card memory (SD Card 
V1.0, V1.1, V2.0 and SDHC (High Capacity) protocol)that could be mounted on the 
STM3210E-EVAL board.

The example provides different SD Card transfer states and operations and here
below a description of each step:
  - First the SDIO peripheral and SD Card are initialized using the SD_Init()
    function: 
            - Configure the SDIO according to the desired SDIO_CK clock frequency.
            - Reset the SD Card
            - Identify the SD Card
            - Initializes the SD Card
            - Get the SD Card Info
            - Select the SD Card
            - Enable the Wide Bus mode (4-bit data)

  - SD Card Erase Operation
            - Starts an Erase operation: Erase the corresponding blocks using 
              the SD_Erase() function 
            - Read the Erased blocks using the SD_ReadMultiBlocks() function
            - Test if the corresponding Blocks are well erased: check if the 
              EraseStatus variable is equal to PASSED. If the erase operation is
              passed LED1 is turned ON.
            
  - SD Card Single Block Operation
            - Starts a Write operation: Write a single Block using the SD_WriteBlock()
              function 
            - Read a single Block using the SD_ReadBlock() function
            - Compare the written Block and the read one: check if the TransferStatus1 
              variable is equal to PASSED. If the Single Block Read/write operation
              is passed LED2 is turned ON.

  - SD Card Multiple Block Operation
            - Starts a Multiple Write operation: Write a multi Blocks using the 
              SD_WriteMultiBlocks() function. 
            - Read a multiple Blocks using the SD_ReadMultiBlocks() function
            - Compare the written Blocks and the read one: check if the TransferStatus2 
              variable is equal to PASSED. If the Multiple Blocks Read/Write operation
              is passed LED3 is turned ON.

All data transfers are made by DMA.
At each operation, the SD Card presence and status is checked using the SD_GetStatus()
function and a global variable "Status" storing the results of the last operation.

SD Card Operations and Written/Read data correctness is signaled  by LED 
lightening and though as follow:
   - If the Erase operation is PASSED then LED1 ON else the LED4 is ON and LED1 is OFF 
   - If the Single Block Write/Read operation is PASSED then LED2 ON else the LED4 is ON and LED2 is OFF
   - If the Multi Blocks Write/Read operation is PASSED then LED3 ON else the LED4 is ON and LED3 is OFF
   - Any SD Card operation including the SD Initialization error is signaled by
     LED4 ON.
 

@par Directory contents 

  - SDIO/uSDCard/stm32f10x_conf.h    Library Configuration file
  - SDIO/uSDCard/stm32f10x_it.c      Interrupt handlers
  - SDIO/uSDCard/stm32f10x_it.h      Header for stm32f10x_it.c
  - SDIO/uSDCard/main.c              Main program
  - SDIO/uSDCard/system_stm32f10x.c  STM32F10x system source file
  
@par Hardware and Software environment 

  - This example runs only on STM32F10x High-Density and XL-Density Devices.
  
  - This example has been tested with STMicroelectronics STM3210E-EVAL (High-Density
    and XL-Density) evaluation board and can be easily tailored to any other 
    supported device and development board.

@note Make sure that the Jumper 17 (JP17) is closed and Jumper 20 (JP20) is open
      in STM3210E-EVAL 


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
