import ctypes
import sys
from remcu_include.STM32F10X_MD_StdPeriph_Lib import *

if sys.platform.startswith('win32'): 	#Windows
    remcu = ctypes.WinDLL("remcu.dll")
elif sys.platform.startswith('cygwin'): #Windows/Cygwin
    remcu = ctypes.WinDLL("remcu.dll")
elif sys.platform.startswith('linux'):  #Linux OS
    remcu = ctypes.CDLL("./libremcu.so")
elif sys.platform.startswith('darwin'): #MacOS
    remcu = ctypes.CDLL("./libremcu.dylib")

success = remcu.remcu_connect2GDB("127.0.0.1", 3333, 3)
if success == 0:
    print("Server error. Possible solutions: ")
    print("1. Check connection to debug server")
    print("2. Check debug server running")
    print("3. Run the script using Python2")
    exit()

remcu.remcu_resetRemoteUnit(1)
"""
/*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f10x_xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       remcu_include/system_stm32f10x.c file
       */
"""
remcu.SystemInit()

remcu.RCC_ADCCLKConfig(RCC_PCLK2_Div4);
remcu.RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE)
remcu.RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE)

GPIO_InitStructure = GPIO_InitTypeDef()

#/* Configure PA.04 (ADC Channel4) as analog input -------------------------*/
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN
remcu.GPIO_Init(GPIOA, GPIO_InitStructure.ref())

ADC_InitStructure = ADC_InitTypeDef()
#/* ADC1 configuration ------------------------------------------------------*/
ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
ADC_InitStructure.ADC_ScanConvMode = DISABLE;
ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
ADC_InitStructure.ADC_NbrOfChannel = 1;
remcu.ADC_Init(ADC1,  ADC_InitStructure.ref());

#/* ADC1 regular channel14 configuration */ 
remcu.ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 1, ADC_SampleTime_55Cycles5);

#/* Enable ADC1 */
remcu.ADC_Cmd(ADC1, ENABLE);

#/* Enable ADC1 reset calibration register */   
remcu.ADC_ResetCalibration(ADC1);
#/* Check the end of ADC1 reset calibration register */
while remcu.ADC_GetResetCalibrationStatus(ADC1) != 0: pass

#/* Start ADC1 calibration */
remcu.ADC_StartCalibration(ADC1);
#/* Check the end of ADC1 calibration */
while remcu.ADC_GetCalibrationStatus(ADC1) != 0: pass

#/* Start ADC1 Software Conversion */ 
remcu.ADC_SoftwareStartConvCmd(ADC1, ENABLE)

def ADC_value():
	#// start conversion (will be endless as we are in continuous mode)
	remcu.ADC_SoftwareStartConvCmd(ADC1, ENABLE)
	return remcu.ADC_GetConversionValue(ADC1)


while True:
	value = ADC_value()
	line = (value*80)/4096
	line = int(line)
	print "value : ", value, " : ", "#"*line
