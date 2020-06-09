import ctypes
import sys
import platform
from inspect import currentframe, getframeinfo

from remcu_include.STM32F4XX_StdPeriph_Lib import *

print("Sys version : ", sys.version)

import sys
if sys.version_info.major == 3:
    raise Exception("Use Python2 only to run this test")

if ctypes.sizeof(ctypes.c_voidp) != 8:
    raise Exception("Use Python2 64-bit build only to run this test")

print("Platform: ", platform.platform())

if sys.platform.startswith('win32'): #Windows
    remcu = ctypes.WinDLL("remcu.dll")
elif sys.platform.startswith('cygwin'): #Windows/Cygwin
    remcu = ctypes.WinDLL("remcu.dll")
elif sys.platform.startswith('linux'):  #Linux OS
    remcu = ctypes.CDLL("./libremcu.so")
elif sys.platform.startswith('darwin'): #MacOS
    remcu = ctypes.CDLL("./libremcu.dylib")

# Before that, plug STM32F4 Discovery board to PC and run openocd utility in command line:
# openocd.exe -f board/stm32f4discovery.cfg
#

debug_server_ip = "127.0.0.1"
default_openocd_port = 6666
default_openocd_gdb_port = 3333
default_jlink_port = 2331
default_stlink_port = 61234
#custom_port = ...
timeout_sec = 3 # It can not be negative

success = remcu.remcu_connect2OpenOCD(debug_server_ip, default_openocd_port, timeout_sec)

# If you use GDB server (either OpenOCD or ST-Link GDB server or Jlink GDB server etc.), you uncomment code below and run it:
# For OpenOCD
#success = remcu.remcu_connect2GDB(debug_server_ip, default_openocd_gdb_port, timeout_sec)
# For Jlink
#success = remcu.remcu_connect2GDB(debug_server_ip, default_jlink_port, timeout_sec)
# For ST link GDB server
#success = remcu.remcu_connect2GDB(debug_server_ip, default_stlink_port, timeout_sec)

if success == 0:
	print("OpenOCD connection failed!... Try to connect GDB server: localhost 3333 port")
	success = remcu.remcu_connect2GDB(debug_server_ip, default_openocd_gdb_port, timeout_sec)
	if success == 0:
		print("GDB connection failed!. Possible solutions: ")
		print("1. Check connection to debug server")
		print("2. Check debug server running")
		exit()

print("Platform: ", platform.platform())
print("Sys version : ", sys.version)

print("Connection status: ", success)

connected = remcu.remcu_isConnected()

if success == 0:
	print("remcu_isConnected: ", connected)
	exit()

cf = currentframe()
print(cf.f_lineno , ". check error: ", remcu.remcu_getErrorCount())

success = remcu.remcu_debuggerTest()
print("Result of debugger test: ", success)
if success != 0:
	print("Debugger test failed: ")
	exit()

remcu.remcu_resetRemoteUnit(__HALT)

print(cf.f_lineno , ". check error: ", remcu.remcu_getErrorCount())

remcu.SystemInit();

print(cf.f_lineno , ". check error: ", remcu.remcu_getErrorCount())

remcu.RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

print(cf.f_lineno , ". check error: ", remcu.remcu_getErrorCount())

GPIO_InitStructure = GPIO_InitTypeDef()

GPIO_InitStructure = GPIO_InitTypeDef()
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz
GPIO_InitStructure.GPIO_OType = GPIO_OType_PP
GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP
remcu.GPIO_Init(GPIOD, GPIO_InitStructure.ref())

print(cf.f_lineno , ". check error: ", remcu.remcu_getErrorCount())

#/* Connect TIM4 pins to AF2 */  
remcu.GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_TIM4)
remcu.GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_TIM4)

print(cf.f_lineno , ". check error: ", remcu.remcu_getErrorCount())

remcu.RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE)

print(cf.f_lineno , ". check error: ", remcu.remcu_getErrorCount())

TIM_TimeBaseStructure = TIM_TimeBaseInitTypeDef()
TIM_OCInitStructure = TIM_OCInitTypeDef()
TIM_TimeBaseStructure.TIM_Period = 0xFFFF
TIM_TimeBaseStructure.TIM_Prescaler = 0xFFFF # TIM4CLK = PCLK1 / 0xFFFF
TIM_TimeBaseStructure.TIM_ClockDivision = 0xFFFF
TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up

remcu.TIM_TimeBaseInit(TIM4, TIM_TimeBaseStructure.ref());

print(cf.f_lineno , ". check error: ", remcu.remcu_getErrorCount())

TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1
TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable
TIM_OCInitStructure.TIM_Pulse = 0
TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High

remcu.TIM_OC1Init(TIM4, TIM_OCInitStructure.ref());

TIM_OCInitStructure.TIM_Pulse = 0xFFFF

remcu.TIM_OC2Init(TIM4, TIM_OCInitStructure.ref());

print(cf.f_lineno , ". check error: ", remcu.remcu_getErrorCount())

remcu.TIM_Cmd(TIM4, ENABLE);

print(cf.f_lineno , ". check error: ", remcu.remcu_getErrorCount())

remcu.TIM_SetCompare1(TIM4, 0) #Set Duty Cycle of TIM4 channel 1
remcu.TIM_SetCompare2(TIM4, 0xFFFF) #Set Duty Cycle of TIM4 channel 2

print(cf.f_lineno , ". check error: ", remcu.remcu_getErrorCount())

print("CR1: ", remcu.get_TIM_CR1(TIM4))

print("TIM_GetCounter: ", remcu.TIM_GetCounter(TIM4))
print("TIM_GetCounter: ", remcu.TIM_GetCounter(TIM4))
print("TIM_GetCounter: ", remcu.TIM_GetCounter(TIM4))
print("TIM_GetCounter: ", remcu.TIM_GetCounter(TIM4))

import time

def toogle_led(times):
	while times != 0:
		remcu.TIM_SetCompare1(TIM4, 0) 
		remcu.TIM_SetCompare2(TIM4, 0xFFFF)
		time.sleep(1)
		remcu.TIM_SetCompare1(TIM4, 0xFFFF) 
		remcu.TIM_SetCompare2(TIM4, 0)
		time.sleep(1)
		times -= 1

for x in range(1,5):
	t1 = remcu.TIM_GetCounter(TIM4)
	t2 = remcu.TIM_GetCounter(TIM4)
	if t1 != t2:
		print("TIM4 test is success!")
		toogle_led(7)
		exit()

print("TIM4 test is failed!")
