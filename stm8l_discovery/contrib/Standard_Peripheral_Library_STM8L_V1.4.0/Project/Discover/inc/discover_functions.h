/**
  ******************************************************************************
  * @file   discover_functions.h
  * @author  Microcontroller Division
  * @version V1.2.4
  * @date    01/2011
  * @brief   This file contains measurement values and boa
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DISCOVER_FUNCTIONS_H
#define __DISCOVER_FUNCTIONS_H

/* Includes ------------------------------------------------------------------*/
#include "stm8l15x.h"

#define STR_VERSION     tab[1] = 'V';tab[2] = '1'|DOT; tab[3] = '2'|DOT; tab[4] = '4'

#define STATE_VREF	0
#define STATE_ICC_RUN 	1
#define STATE_LPR_LCD 	2
#define STATE_LPR 	3
#define STATE_HALT 	4
#define MAX_STATE 	5


/* Theorically BandGAP 1.224volt */
#define VREF 		1.224L


/* UNCOMMENT the line below for use the VREFINT_Factory_CONV value*/ 
/* else we use the typical value defined in the datasheet (see Vdd_appli function in the file discover_functions.c) */

// #define VREFINT_FACTORY_CONV 1

/*
	ADC Converter 
	LSBIdeal = VREF/4096 or VDA/4096
*/
#define ADC_CONV 	4096

/*
 VDD Factory for VREFINT measurement 
*/
#define VDD_FACTORY 3.0L

/* The VREFINT_Factory_CONV byte represents the LSB of the VREFINT 12-bit ADC conversion result. The MSB have a fixed value: 0x6 */

#define VREFINT_Factory_CONV_ADDRESS ((uint8_t*)0x4910)

/* 
	* The Typical value is 1.224 
	* Min value 1.202
	* Max value	1.242
	* The value VREF is 0x668 to 0x69f 
	*
*/
#define VREFINT_Factory_CONV_MSB 0x600 /* Le value MSB always 0x600 */
#define VREFINT_Factory_CONV_MIN 0x60 /* Low byte min */
#define VREFINT_Factory_CONV_MAX 0xA0 /* Low byte max */

#define MAX_CURRENT 	9999

/* AUTO TEST VALUE */

#define VCC_MIN 	2915 /* nominal Vcc/Vdd is 2.99V, allow 2.5% lower - Vref can be ~2% lower than 1.225 */
#define VCC_MAX 	3100
#define ICC_RUN_MIN 	1000
#define ICC_RUN_MAX 	1600 /* typical ICC_RUN is ~1.3mA, allow ~15% bigger */
#define ICC_HALT_MIN 	300
#define ICC_HALT_MAX 	800 /* typical ICC_HALT is 0.35uA, allow 800 nA instead 1000  this low value is for select the best circuits*/
#define ICC_LP_MIN 	2500
#define ICC_LP_MAX 	4060 /* typical ICC_LP is ~2.9uA, allow ~40% bigger */
#define LSE_DELAY 	2000

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
int _fctcpy(char name);
void convert_into_char(uint16_t number, uint16_t *p_tab);
void LPR_init(void);
void Halt_Init(void);
uint16_t Vref_measure(void);
void Icc_measure(void);
float Icc_measure_RUN(void);
float Icc_measure_HALT(void);
float Icc_measure_LPR(void);
void Icc_measure_LPR_LCD(void);	
void auto_test(void);
void Bias_measurement(void);
void test_vdd(void);
void test_icc_Run(void);
void test_icc_LP(void);
void test_icc_HALT(void);
void display_MuAmp (uint16_t);
void FLASH_ProgramBias(uint8_t) ;
float Vdd_appli(void);
	
#endif /* __DISCOVER_FUNCTIONS_H*/

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
