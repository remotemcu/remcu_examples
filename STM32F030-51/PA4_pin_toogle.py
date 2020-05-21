import ctypes
import sys

from remcu_include.STM32F030_StdPeriph_Lib import *

if sys.platform.startswith('win32'): #Windows
    remcu = ctypes.WinDLL("remcu.dll")
elif sys.platform.startswith('cygwin'): #Windows/Cygwin
    remcu = ctypes.WinDLL("remcu.dll")
elif sys.platform.startswith('linux'):  #Linux OS
    remcu = ctypes.CDLL("./libremcu.so")
elif sys.platform.startswith('darwin'): #MacOS
    remcu = ctypes.CDLL("./libremcu.dylib")
    
    
debug_server_ip = "127.0.0.1"
default_GDB_port = 3333
timeout_sec = 3 # It can not be negative

success = remcu.remcu_connect2GDB(debug_server_ip, default_GDB_port, timeout_sec)
if success == 0:
    print("Server error. Possible solutions: ")
    print("1. Check connection to debug server")
    print("2. Check debug server running")
    print("3. Run the script using Python2")
    exit()
    
remcu.remcu_resetRemoteUnit(__HALT);

remcu.SystemInit();

remcu.RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

GPIO_InitStructure = GPIO_InitTypeDef()
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
remcu.GPIO_Init(GPIOA, GPIO_InitStructure.ref());

from time import sleep

while True:
    remcu.GPIO_SetBits(GPIOA,GPIO_Pin_4)
    sleep(0.1)
    remcu.GPIO_ResetBits(GPIOA, GPIO_Pin_4)
    sleep(0.1)
    print("PA4 pin is toogling...")

