#ifndef UTILS_H
#define UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "remcu.h"
#include "stm32f30x.h"
//#include "stm32f3_discovery.h"
#include "stm32f3_discovery_lsm303dlhc.h"
#include "stm32f3_discovery_l3gd20.h"
//#include "hw_config.h"
//#include "platform_config.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void Demo_GyroConfig(void);
void Demo_GyroReadAngRate (float* pfData);
void Demo_CompassConfig(void);
void Demo_CompassReadMag (float* pfData);
void Demo_CompassReadAcc(float* pfData);

#ifdef __cplusplus
}
#endif

#endif // UTILS_H
