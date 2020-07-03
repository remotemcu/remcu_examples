/**
  ******************************************************************************
  * @file    k_storage.h
  * @author  MCD Application Team
  * @brief   Header for k_storage.c file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2016 STMicroelectronics International N.V. All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __K_STORAGE_H
#define __K_STORAGE_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
typedef enum {
  KSTORAGE_NOERROR,
  KSTORAGE_ERROR_LINK,
  KSTORAGE_ERROR_MOUNT,
  KSTORAGE_ERROR_OPEN,
  KSTORAGE_ERROR_READ,
  KSTORAGE_ERROR_WRITE,
  KSTORAGE_ERROR_CLOSE,
  KSTORAGE_FIND_DIRDOESNTEXSIT,
  KSTORAGE_FIND_NOFILE,
  KSTORAGE_FIND_NOPREV,
  KSTORAGE_ERROR_MALLOC
} STORAGE_RETURN;

enum {
  KSTORAGE_FINDFIRST,
  KSTORAGE_FINDNEXT,
  KSTORAGE_FINDPREV,
  KSTORAGE_FINDCLOSE,
};
/* Exported functions ------------------------------------------------------- */
STORAGE_RETURN kStorage_Init(void);
STORAGE_RETURN kStorage_DeInit(void);
STORAGE_RETURN kStorage_OpenFileDrawBMP(uint16_t xpos, uint16_t ypos, uint8_t *BmpName);
STORAGE_RETURN kStorage_OpenFileDrawPixel(uint16_t xpos, uint16_t ypos, uint8_t *BmpName);
STORAGE_RETURN kStorage_FileExist(uint8_t *filename);
STORAGE_RETURN kStorage_GetFileInfo(uint8_t *filename,FILINFO* fileinfo);
STORAGE_RETURN kStorage_GetDirectoryFiles(const uint8_t *DirName, uint8_t action, uint8_t *FileName, uint8_t *FileExt);

uint8_t  kStorage_GetStatus (void);
void     kStorage_SdDetection(uint8_t status);
uint32_t kStorage_GetCapacity (void);
uint32_t kStorage_GetFree (void);

#ifdef __cplusplus
}
#endif

#endif /*__K_STORAGE_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
