/**
  ******************************************************************************
  * @file    FreeRTOS\FreeRTOS_LowPower_LPTIM\Src\low_power_tick_management_LPTIM.c
  * @author  MCD Application Team
  * @brief   RTOS Low power tick management using LPTIM peripheral.    
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



#include "cmsis_os.h"
#include "stm32l0xx_hal.h"
#include "stm32l0xx_timebase_lptim.h"

#define portMAX_16_BIT_NUMBER				( 0xffffUL )

extern LPTIM_HandleTypeDef             LptimHandle;

void vPortSetupTimerInterrupt( void );

#if( configUSE_TICKLESS_IDLE == 2 )

/*
 * Override the default definition of vPortSuppressTicksAndSleep() that is
 * weakly defined in the FreeRTOS Cortex-M port layer with a version that
 * manages the LPTIM clock, as the tick is generated from the low power LPTIM
 * and not the SysTick as would normally be the case on a Cortex-M.
 */
void vPortSuppressTicksAndSleep( TickType_t xExpectedIdleTime );


/*-----------------------------------------------------------*/

/* Calculate how many clock increments make up a single tick period. */
static const uint32_t ulReloadValueForOneTick = ( LSE_VALUE / configTICK_RATE_HZ);

/* Will hold the maximum number of ticks that can be suppressed. */
static uint32_t xMaximumPossibleSuppressedTicks = 0;

/* Flag set from the tick interrupt to allow the sleep processing to know if
sleep mode was exited because of a timer interrupt or a different interrupt. */
volatile uint32_t ulTickFlag = pdFALSE;

#endif


/*-----------------------------------------------------------*/

void vPortSetupTimerInterrupt( void )
{
  InitTick(configLIBRARY_LOWEST_INTERRUPT_PRIORITY);
	
#if( configUSE_TICKLESS_IDLE == 2 )

	/* The maximum number of ticks that can be suppressed depends on the clock
	frequency. */
	xMaximumPossibleSuppressedTicks = portMAX_16_BIT_NUMBER / ulReloadValueForOneTick;
#endif	/* ( configUSE_TICKLESS_IDLE == 2 ) */
}



#if( configUSE_TICKLESS_IDLE == 2 )
/*-----------------------------------------------------------*/

void vPortSuppressTicksAndSleep( TickType_t xExpectedIdleTime )
{
  uint32_t ulReloadValue, ulCompleteTickPeriods, ulCount;
  eSleepModeStatus eSleepAction;

	/* THIS FUNCTION IS CALLED WITH THE SCHEDULER SUSPENDED. */

	/* Make sure the LPTIM reload value does not overflow the counter. */
	if( xExpectedIdleTime > xMaximumPossibleSuppressedTicks )
	{
		xExpectedIdleTime = xMaximumPossibleSuppressedTicks;
	}

	/* Calculate the reload value required to wait xExpectedIdleTime tick
	periods. */
	ulReloadValue = HAL_LPTIM_ReadCounter(&LptimHandle) + (ulReloadValueForOneTick * (xExpectedIdleTime - 1UL));
	
	ulCount = HAL_LPTIM_ReadCounter(&LptimHandle);
	
	HAL_LPTIM_PWM_Stop_IT(&LptimHandle);

	/* Enter a critical section but don't use the taskENTER_CRITICAL() method as
	that will mask interrupts that should exit sleep mode. */
	__asm volatile ( "cpsid i" );
	__asm volatile( "dsb" );
	__asm volatile( "isb" );

	/* The tick flag is set to false before sleeping.  If it is true when sleep
	mode is exited then sleep mode was probably exited because the tick was
	suppressed for the entire xExpectedIdleTime period. */
	ulTickFlag = pdFALSE;

	/* If a context switch is pending then abandon the low power entry as the
	context switch might have been pended by an external interrupt that	requires
	processing. */
	eSleepAction = eTaskConfirmSleepModeStatus();
	if( eSleepAction == eAbortSleep )
	{
		/* Restart tick and continue counting to complete the current time
		slice. */
		HAL_LPTIM_PWM_Start_IT(&LptimHandle, ulCount-1, (ulCount/2));
		/* Re-enable interrupts  */
		__asm volatile ( "cpsie i" );
	}
	else
	{		
		HAL_LPTIM_TimeOut_Start_IT(&LptimHandle, 0xFFFF, ulReloadValue);
		
		/* A user definable macro that allows application code to be inserted
		here.  Such application code can be used to minimise power consumption
		further by turning off IO, peripheral clocks, the Flash, etc. */
		configPRE_STOP_PROCESSING();

		/* There are no running state tasks and no tasks that are blocked with a
		time out.  Assuming the application does not care if the tick time slips
		with respect to calendar time then enter a deep sleep that can only be
		woken by (in this demo case) the user button being pushed on the
		STM32L discovery board.  If the application does require the tick time
		to keep better track of the calender time then the LPTIM peripheral can be
		used to make rough adjustments. */
		HAL_PWR_EnterSTOPMode( PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI );

		/* A user definable macro that allows application code to be inserted
		here.  Such application code can be used to reverse any actions taken
		by the configPRE_STOP_PROCESSING().  In this demo
		configPOST_STOP_PROCESSING() is used to re-initialise the clocks that
		were turned off when STOP mode was entered. */
		configPOST_STOP_PROCESSING();
		
		ulCount = HAL_LPTIM_ReadCounter(&LptimHandle);

		HAL_LPTIM_PWM_Stop_IT(&LptimHandle);

		/* Re-enable interrupts */
    __asm volatile ( "cpsie i" );
		if( ulTickFlag != pdFALSE )
		{
			ulCompleteTickPeriods = xExpectedIdleTime - 1UL;
			
			HAL_LPTIM_PWM_Start_IT(&LptimHandle, ulReloadValueForOneTick -1UL, ulReloadValueForOneTick/2);
		}
		else
		{
			ulCompleteTickPeriods = ulCount/ ulReloadValueForOneTick;
			ulReloadValue = (ulReloadValueForOneTick - ( ulCompleteTickPeriods % ulReloadValueForOneTick ));
			
			HAL_LPTIM_PWM_Start_IT(&LptimHandle, ulReloadValue - 1, ulReloadValue/2);				
		}
			
		/* Wind the tick forward by the number of tick periods that the CPU
		remained in a low power state. */
		vTaskStepTick( ulCompleteTickPeriods );
	}
}
#endif /* ( configUSE_TICKLESS_IDLE == 2 ) */
	

