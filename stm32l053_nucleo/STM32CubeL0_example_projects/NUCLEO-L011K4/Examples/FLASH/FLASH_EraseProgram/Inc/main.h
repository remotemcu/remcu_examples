/**
  ******************************************************************************
  * @file    FLASH/FLASH_EraseProgram/Inc/main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_hal.h"
#include "stm32l0xx_nucleo_32.h"


/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Base address of the Flash sectors */
/* Base address of the Flash pages */
#define ADDR_FLASH_PAGE_0     ((uint32_t)0x08000000) /* Base @ of Page 0, 128 bytes */
#define ADDR_FLASH_PAGE_1     ((uint32_t)0x08000080) /* Base @ of Page 1, 128 bytes */
#define ADDR_FLASH_PAGE_2     ((uint32_t)0x08000100) /* Base @ of Page 2, 128 bytes */
#define ADDR_FLASH_PAGE_3     ((uint32_t)0x08000180) /* Base @ of Page 3, 128 bytes */
#define ADDR_FLASH_PAGE_4     ((uint32_t)0x08000200) /* Base @ of Page 4, 128 bytes */
#define ADDR_FLASH_PAGE_5     ((uint32_t)0x08000280) /* Base @ of Page 5, 128 bytes */
#define ADDR_FLASH_PAGE_6     ((uint32_t)0x08000300) /* Base @ of Page 6, 128 bytes */
#define ADDR_FLASH_PAGE_7     ((uint32_t)0x08000380) /* Base @ of Page 7, 128 bytes */
#define ADDR_FLASH_PAGE_8     ((uint32_t)0x08000400) /* Base @ of Page 8, 128 bytes */
#define ADDR_FLASH_PAGE_9     ((uint32_t)0x08000480) /* Base @ of Page 9, 128 bytes */
#define ADDR_FLASH_PAGE_10    ((uint32_t)0x08000500) /* Base @ of Page 10, 128 bytes */
#define ADDR_FLASH_PAGE_11    ((uint32_t)0x08000580) /* Base @ of Page 11, 128 bytes */
#define ADDR_FLASH_PAGE_12    ((uint32_t)0x08000600) /* Base @ of Page 12, 128 bytes */
/* ... */
#define ADDR_FLASH_PAGE_126   ((uint32_t)0x08003F00) /* Base @ of Page 126, 128 bytes */
#define ADDR_FLASH_PAGE_127   ((uint32_t)0x08003F80) /* Base @ of Page 127, 128 bytes */
    

/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
