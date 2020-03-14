/**
  ******************************************************************************
  * @file    FSMC/OneNAND/main.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32_eval.h"
#include "stm32100e_eval_fsmc_onenand.h"

/** @addtogroup STM32F10x_StdPeriph_Examples
  * @{
  */

/** @addtogroup FSMC_OneNAND
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define OneNAND_SAMSUNG_MANUFACTURER_ID     0x00EC
#define OneNAND_SAMSUNG_DEVICE_ID           0x0025

#define OneNAND_BUFFER_SIZE                 0x0400 /* Page size: 1024 x 16 bits = 2048 Bytes */
#define OneNAND_WRITE_BLOCK_NUMBER          0x0000 /* should be between 0 and 511, the block size is 128 KBytes */
#define OneNAND_WRITE_PAGE_NUMBER           0x0000 /* The page size inside a Block is 2 KBytes */
#define OneNAND_NUMBER_OF_PAGE_PER_BLOCK    0x0040 /* 64 pages per block */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
OneNAND_IDTypeDef OneNAND_ID;
OneNAND_ADDRESS Address;
uint16_t TxBuffer[OneNAND_BUFFER_SIZE], RxBuffer_A[OneNAND_BUFFER_SIZE], RxBuffer_S[OneNAND_BUFFER_SIZE];
uint32_t j = 0, PageIndex = 0, Status = 0;

/* Private function prototypes -----------------------------------------------*/
void Fill_hBuffer(uint16_t *pBuffer, uint16_t BufferLenght, uint32_t Offset);

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f10x_xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f10x.c file
     */
  
  /* Initialize LEDs on STM3220F-EVAL board */
  STM_EVAL_LEDInit(LED1);
  STM_EVAL_LEDInit(LED2);
  STM_EVAL_LEDInit(LED3);  
  STM_EVAL_LEDInit(LED4); 
  
  /* FSMC Initialization */
  OneNAND_Init();

  /* Read OneNAND memory ID */
  OneNAND_ReadID(&OneNAND_ID);

  /* Verify the OneNAND ID */ 
  if((OneNAND_ID.Manufacturer_ID == OneNAND_SAMSUNG_MANUFACTURER_ID) && 
     (OneNAND_ID.Device_ID == OneNAND_SAMSUNG_DEVICE_ID)) 
  {
    /* Fill the buffer to send */
    Fill_hBuffer(TxBuffer, OneNAND_BUFFER_SIZE , 0x320F);
    Address.Block = OneNAND_WRITE_BLOCK_NUMBER;
    Address.Page = OneNAND_WRITE_PAGE_NUMBER;
    

    /***** Erase then write to the OneNAND memory ******************************/  
    /* Unlock the selected OneNAND block */
    Status = OneNAND_UnlockBlock(OneNAND_WRITE_BLOCK_NUMBER);

    if (Status == 0)
    { 
      /* Erase the selected OneNAND block */
      Status = OneNAND_EraseBlock(Address.Block);

      if (Status == 0)
      { 
        /* Write data to the OneNAND memory (128Kbytes by page 2KBytes each) */
        for(PageIndex = 0; (PageIndex < OneNAND_NUMBER_OF_PAGE_PER_BLOCK) && (Status ==0); PageIndex++)
        {
          Status = OneNAND_WriteBuffer(TxBuffer, Address, OneNAND_BUFFER_SIZE);
          Address.Page++;
        }

        if (Status == 0)
        { 
          /***** Verify of the written data using asynchronous read ***********/
          Fill_hBuffer(RxBuffer_A, OneNAND_BUFFER_SIZE , 0xFF);
          Status = 0;
          Address.Block = OneNAND_WRITE_BLOCK_NUMBER;
          Address.Page = OneNAND_WRITE_PAGE_NUMBER;

          for(PageIndex = 0; PageIndex < OneNAND_NUMBER_OF_PAGE_PER_BLOCK; PageIndex++)
          {
            /* Read back the written data (By page) */
            OneNAND_AsynchronousRead(RxBuffer_A, Address, OneNAND_BUFFER_SIZE);
   
            /* Verify the written data */
            for(j = 0; j < OneNAND_BUFFER_SIZE; j++)
            {
              if(TxBuffer[j] != RxBuffer_A[j])
              {
                Status++;
              }
            }
            Address.Page++;
          }
          
          if (Status != 0)
          { 
            /* Turn ON LED2 */
            STM_EVAL_LEDOn(LED2); 
          }

          /***** Verify of the written data using synchronous read ************/   
          Fill_hBuffer(RxBuffer_S, OneNAND_BUFFER_SIZE , 0xFF);
          Status = 0;
          Address.Block = OneNAND_WRITE_BLOCK_NUMBER;
          Address.Page = OneNAND_WRITE_PAGE_NUMBER;

          for(PageIndex = 0; PageIndex < OneNAND_NUMBER_OF_PAGE_PER_BLOCK; PageIndex++)
          {
            /* Read back the written data (By page) */
            OneNAND_SynchronousRead(RxBuffer_S, Address, OneNAND_BUFFER_SIZE);
   
            /* Verify the written data */
            for(j = 0; j < OneNAND_BUFFER_SIZE; j++)
            {
              if(TxBuffer[j] != RxBuffer_S[j])
              {
                Status++;
              }
            }
            Address.Page++;
          }

          if (Status != 0)
          { 
            /* Turn ON LED3 */
            STM_EVAL_LEDOn(LED3);
          }
        }        
        else
        { 
          /* Turn ON LED4 */
          STM_EVAL_LEDOn(LED4); 
        }
      } 
      else
      { 
        /* Turn ON LED2 and LED4*/
        STM_EVAL_LEDOn(LED2); 
        STM_EVAL_LEDOn(LED4); 
      }     
    } 
    else
    { 
      /* Turn ON LED3 and LED4 */
      STM_EVAL_LEDOn(LED3);
      STM_EVAL_LEDOn(LED4); 
    }  
  }
  else
  {
    /* Turn ON LED2, LED3 and LED4 */
    STM_EVAL_LEDOn(LED2);    
    STM_EVAL_LEDOn(LED3);
    STM_EVAL_LEDOn(LED4);
  }

  if (Status == 0)
  { 
   /* Turn ON LED1 */
   STM_EVAL_LEDOn(LED1);
  }
  		  
  /* Infinite loop */
  while (1)
  {
  }
}

/**
  * @brief  Fills a global 16-bit buffer
  * @param  pBuffer: pointer on the Buffer to fill
  * @param  BufferSize: size of the buffer to fill
  * @param  Offset: first value to fill on the Buffer
  * @retval None
  */
void Fill_hBuffer(uint16_t *pBuffer, uint16_t BufferLenght, uint32_t Offset)
{
  uint16_t IndexTmp = 0;

  /* Put in global buffer different values */
  for (IndexTmp = 0; IndexTmp < BufferLenght; IndexTmp++ )
  {
    pBuffer[IndexTmp] = IndexTmp + Offset;
  }
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}

#endif

/**
  * @}
  */


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
