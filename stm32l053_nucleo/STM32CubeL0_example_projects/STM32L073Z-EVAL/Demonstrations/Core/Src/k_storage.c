/**
  ******************************************************************************
  * @file    k_storage.c
  * @author  MCD Application Team   
  * @brief   This file provides the kernel storage functions
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stdlib.h"
#include "stdio.h"
#include "k_config.h"
#include "k_storage.h"
#include "k_window.h"
#include "string.h" 

/** @addtogroup CORE
  * @{
  */

/** @defgroup KERNEL_STORAGE
  * @brief Kernel storage routines
  * @{
  */

extern void     hx8347d_SetCursor(uint16_t Xpos, uint16_t Ypos);
/* External variables --------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static FATFS mSDDISK_FatFs;         /* File system object for USB disk logical drive */
static char mSDDISK_Drive[4];       /* USB Host logical drive number */

static uint8_t StorageStatus;
/* Private function prototypes -----------------------------------------------*/
static void kStorage_GetExt(char * pFile, char * pExt);
/* Private functions ---------------------------------------------------------*/


/**
  * @brief  Storage drives initialization
  * @param  None 
  * @retval None
  */
STORAGE_RETURN kStorage_Init(void)
{
  /* Link the micro SD disk I/O driver */
  if(FATFS_LinkDriver(&SD_Driver, mSDDISK_Drive)!=0)
  {
    return KSTORAGE_ERROR_LINK;
  }
  /* Register the file system object to the FatFs module */
  if(f_mount(&mSDDISK_FatFs, (TCHAR const*)mSDDISK_Drive, 0) != FR_OK)
  {
    /* FatFs Initialization Error */
    return KSTORAGE_ERROR_MOUNT;
  }
  return KSTORAGE_NOERROR;
}

/**
  * @brief  Storage drives initialization
  * @param  None 
  * @retval None
  */
STORAGE_RETURN kStorage_DeInit(void)
{
  /* Unlink the RAM disk I/O driver */
  FATFS_UnLinkDriver(mSDDISK_Drive);
  return KSTORAGE_NOERROR;
}

/**
  * @brief  Storage get status
  * @param  None
  * @retval int
  */
uint8_t kStorage_GetStatus(void)
{  
  return StorageStatus;
}


/**
  * @brief  Storage get free space
  * @param  unit: 
  * @retval int
  */
uint32_t kStorage_GetFree(void)
{ 
  uint32_t   fre_clust = 0;
  FATFS *fs ;
  FRESULT res = FR_INT_ERR;
  
  fs = &mSDDISK_FatFs;
  res = f_getfree("1:", (DWORD *)&fre_clust, &fs);

  if(res == FR_OK)
  {
    return (fre_clust * fs->csize);
  }
  else
  {
    return 0;
  }
}

/**
  * @brief  SD detect callback
  * @param  None
  * @retval None
  */ 
void kStorage_SdDetection(uint8_t status)
{
  StorageStatus = status;
  if(status == SD_NOT_PRESENT)
  {
     kWindow_Error("SD card removed\ninsert SD card\nand\nReset the board\n");
     while(1);
  }
}

/**
  * @brief  Return the first file present inside a direcory with FileExt
  * @param  DirName: Directory name
  * @param  Action :  Opendir, previous or next files, Closedir
  * @param  FileExt:  extension filter if NULL not filter
  * @retval STORAGE_RETURN
  */
STORAGE_RETURN kStorage_GetDirectoryFiles(const uint8_t *DirName, uint8_t action, uint8_t *FileName, uint8_t *FileExt)
{
  static DIR MyDirectory;
  static uint16_t count = 0;
  uint16_t prev = 0;
  FILINFO MyFileInfo;
  uint8_t ext[4];
  
  switch(action)
  {
  case KSTORAGE_FINDPREV :
    /* Recherche du prev */
    if(count == 1 ) return KSTORAGE_FIND_NOPREV;
    prev = count-1;
    count = 0;
    f_closedir(&MyDirectory);
  case KSTORAGE_FINDFIRST :
    if(f_opendir(&MyDirectory, (char const *)DirName) != FR_OK)
    {
      return KSTORAGE_FIND_DIRDOESNTEXSIT;
    }
  case KSTORAGE_FINDNEXT :     
    do 
    {
      /* Get the first file */
      if((f_readdir(&MyDirectory, &MyFileInfo) != FR_OK) || (MyFileInfo.fname[0] == '\0'))
      {
        return KSTORAGE_FIND_NOFILE;
      }
      
      /* check the file extension */
      kStorage_GetExt(MyFileInfo.fname, (char *)ext);
      
      /* part to handle previous */
      if((prev != 0 ) && ((FileExt == NULL) || (strcmp((char const*)ext, (char const*)FileExt) == 0)))
      {
          count++;
          if(count != prev)
          {
            /* go to next picture */
            ext[0] = '\0';
          }
          else
          {
            count--;
          }
      }
    } 
    while((FileExt != NULL) && strcmp((char const*)ext, (char const*)FileExt) != 0);
    count++;
    strcpy((char *)FileName,MyFileInfo.fname);
    break;
  case KSTORAGE_FINDCLOSE : 
    f_closedir(&MyDirectory);
    break;
  }
  
  return KSTORAGE_NOERROR;
}

/**
  * @brief  Check file prescence
  * @param  file : file name
  * @retval KSTORAGE_NOERROR else an error has been detected
  */
STORAGE_RETURN kStorage_FileExist(uint8_t *filename)
{
  FIL F1;
  
  if(f_open (&F1, (char *)filename, FA_READ) != FR_OK)
  {
    return KSTORAGE_ERROR_OPEN;
  }
  
  f_close(&F1);
  return KSTORAGE_NOERROR;
}

/**
  * @brief  Check file prescence
  * @param  file : file name
  * @param  fileinfo : Information about the file
  * @retval KSTORAGE_NOERROR else an error has been detected
  */
STORAGE_RETURN kStorage_GetFileInfo(uint8_t *filename,FILINFO* fileinfo)
{
  if(f_stat((char *)filename, fileinfo) != FR_OK )
  {
    return KSTORAGE_ERROR_OPEN;
  }
  return KSTORAGE_NOERROR;
}

/**
  * @brief  Open a file and use BMP Draw to display the image (limited for 8k images)
  * @param  xpos: x position for the image 
  * @param  xpos: y position for the image
  * @param  BmpName : file name
  * @retval KSTORAGE_NOERROR else an error has been detected
  */
STORAGE_RETURN kStorage_OpenFileDrawBMP(uint16_t xpos, uint16_t ypos, uint8_t *BmpName)
{
  unsigned int BytesRead;
  FIL F1;
  uint8_t *kstorage_tmpbuffer = NULL;
  
  if(f_open (&F1, (char *)BmpName, FA_READ) != FR_OK)
  {
    return KSTORAGE_ERROR_OPEN;
  }
    
  /* Memory allocation to get picture data (limited to 8k*/
  kstorage_tmpbuffer = malloc(sizeof(uint8_t)*(8192+1024));
  if(kstorage_tmpbuffer==NULL)
  {
    f_close(&F1);
    return KSTORAGE_ERROR_MALLOC;
  }
  
  /* Read the data file content limited to size 8192+1024 */
  if(f_read (&F1,(uint8_t*) kstorage_tmpbuffer, sizeof(uint8_t)*(8192+1024), &BytesRead) != FR_OK)
  {
    f_close(&F1);
    free(kstorage_tmpbuffer);
    return KSTORAGE_ERROR_READ;
  }
  
  /* Display the image */
  BSP_LCD_DrawBitmap(xpos, ypos, kstorage_tmpbuffer); 
  free(kstorage_tmpbuffer);
  f_close(&F1);
  
  return KSTORAGE_NOERROR;
}


/**
  * @brief  Open a file and draw each pixel of the corresponding file
  * @param  xpos: x position for the image 
  * @param  xpos: y position for the image
  * @param  BmpName : file name
  * @retval err: Error status (0=> success, 1=> fail)
  */
STORAGE_RETURN kStorage_OpenFileDrawPixel(uint16_t xpos, uint16_t ypos, uint8_t *BmpName)
{
  uint32_t index, size, width, height; 

  uint32_t bmpaddress, bit_pixel;
  unsigned int BytesRead;
  FIL file1;
  uint8_t *kstorage_tmpbuffer = NULL;
  
  if(f_open(&file1, (char *)BmpName, FA_READ) != FR_OK)
  {
    return KSTORAGE_ERROR_OPEN;
  }
  
  /* Memory allocation for the BMP header */
  kstorage_tmpbuffer = malloc(sizeof(uint8_t)*30);
  if(kstorage_tmpbuffer==NULL)
  {
    f_close(&file1);
    return KSTORAGE_ERROR_MALLOC;
  }

  /* Read BMP header */
  if(f_read(&file1, (char *)kstorage_tmpbuffer, 30, &BytesRead) != FR_OK)
  {
    free(kstorage_tmpbuffer);
    f_close(&file1);
    return KSTORAGE_ERROR_READ;
  }
    
  /* get the bmpadress */
  bmpaddress = (uint32_t)kstorage_tmpbuffer;
  
  /* Read bitmap size */
  size  =  *(uint16_t *) (bmpaddress + 2);
  size |= (*(uint16_t *) (bmpaddress + 4)) << 16;

  /* Get bitmap data address offset */
  index  =  *(uint16_t *) (bmpaddress + 10);
  index |= (*(uint16_t *) (bmpaddress + 12)) << 16;
  
  /* Read bitmap width */
  width  =  *(uint16_t *) (bmpaddress + 18);
  width |= (*(uint16_t *) (bmpaddress + 20)) << 16;
  
  /* Read bitmap height */
  height  =  *(uint16_t *) (bmpaddress + 22);
  height |= (*(uint16_t *) (bmpaddress + 24)) << 16;
  
  /* Read bit/pixel */
  bit_pixel = (*(uint16_t *) (bmpaddress + 28)) / 8;  
  // bit_pixel = bit_pixel/8;
  
  size = (size - index);
  xpos = xpos + height - 1;
  /* Synchronize f_read right in front of the image data */
  f_close (&file1);  
 

  /* Memory allocation for the BMP data line */
  free(kstorage_tmpbuffer);
  kstorage_tmpbuffer = malloc(sizeof(uint8_t)*width*bit_pixel);
  if(kstorage_tmpbuffer==NULL)
  {
    return KSTORAGE_ERROR_OPEN;
  }

  f_open (&file1, (TCHAR const*)BmpName, FA_READ);
  
  f_read(&file1, kstorage_tmpbuffer, index, &BytesRead);

  do {
    /* read a line */
    f_read(&file1, kstorage_tmpbuffer, width*bit_pixel, &BytesRead);
    size -= width*bit_pixel;

 /* Set Cursor */
  hx8347d_SetCursor(xpos, ypos);
  /* Prepare to write GRAM */
  LCD_IO_WriteReg(LCD_REG_34);
  
  /* Write 16-bit GRAM Reg */
  LCD_IO_WriteMultipleData(kstorage_tmpbuffer, width*bit_pixel);
    
    xpos--;
  } while (size != 0);

  /* Exit : free memory, close the file */
  free(kstorage_tmpbuffer);
  f_close(&file1);
  
  return KSTORAGE_NOERROR;
}      


/**
  * @brief  Return file Extension
  * @param  pFile
  * @param  pExt
  * @retval None
  */
void kStorage_GetExt(char * pFile, char * pExt) 
{
  uint8_t i = 0, j = 0;
  
  pExt[0] = '\0';
  
  /* Search beginning of extension */
  while(pFile[i] != '.' && pFile[i] != '\0')
  { i++; };
  
  if(pFile[i] == '\0') return;
  i++;
  /* Copy extension */
  do {
    pExt[j] = pFile[i];
    j++; i++;
  } while(pFile[i]!= '\0');
  pExt[j] = '\0';
}

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
