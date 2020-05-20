import ctypes
import sys

from remcu_include.STM32F051_StdPeriph_Lib import *

if sys.platform.startswith('win32'): #Windows
    remcu = ctypes.WinDLL("remcu.dll")
elif sys.platform.startswith('cygwin'): #Windows/Cygwin
    remcu = ctypes.WinDLL("remcu.dll")
elif sys.platform.startswith('linux'):  #Linux OS
    remcu = ctypes.CDLL("./libremcu.so")
elif sys.platform.startswith('darwin'): #MacOS
    remcu = ctypes.CDLL("./libremcu.dylib")
    
    
debug_server_ip = "127.0.0.1"
default_openocd_port = 6666
timeout_sec = 3 # It can not be negative

success = remcu.remcu_connect2OpenOCD(debug_server_ip, default_openocd_port, timeout_sec)
if success == 0:
    print("Server error. Possible solutions: ")
    print("1. Check connection to debug server")
    print("2. Check debug server running")
    print("3. Run the script using Python2")
    exit()
    
remcu.remcu_resetRemoteUnit(__HALT);

remcu.SystemInit();

#GPIOC Periph clock enable:
remcu.RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE)

#enable ADC system clock:
remcu.RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE)

GPIO_InitStructure = GPIO_InitTypeDef()

#/* Configure PA.05 (ADC Channel5) as analog input -------------------------*/
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5
GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN
remcu.GPIO_Init(GPIOA, GPIO_InitStructure.ref())

#define ADC base config:

#/* ADCs DeInit */  
remcu.ADC_DeInit(ADC1);

ADC_InitStructure = ADC_InitTypeDef()

#/* Initialize ADC structure */
remcu.ADC_StructInit(ADC_InitStructure.ref())

#/* Configure the ADC1 in continuous mode with a resolution equal to 12 bits  */
ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b
ADC_InitStructure.ADC_ContinuousConvMode = ENABLE
ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None
ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right
ADC_InitStructure.ADC_ScanDirection = ADC_ScanDirection_Upward
remcu.ADC_Init(ADC1,  ADC_InitStructure.ref());

#/* ADC1 regular channel5 configuration */ 
remcu.ADC_ChannelConfig(ADC1, ADC_Channel_5, ADC_SampleTime_239_5Cycles)

#/* ADC Calibration */
remcu.ADC_GetCalibrationFactor(ADC1)

#/* Enable ADC1 */
remcu.ADC_Cmd(ADC1, ENABLE)

#/* Check the end of ADC1 reset calibration register */
while remcu.ADC_GetFlagStatus(ADC1, ADC_FLAG_ADRDY) == 0: pass

#/* Start ADC1 Software Conversion */ 
remcu.ADC_StartOfConversion(ADC1)

def ADC_value():
    #// start conversion (will be endless as we are in continuous mode)
    while remcu.ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET: pass
    return remcu.ADC_GetConversionValue(ADC1)

while True:
    value = ADC_value()
    line = (value*80)/4096
    line = int(line)
    print "value : ", value, " : ", "#"*line