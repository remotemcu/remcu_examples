/** @page COMP2_FrequencyMeasurement  Use Comparator 2 to measure external signal frequency and duty cycle.

  @verbatim
  ******************** (C) COPYRIGHT 2013 STMicroelectronics *******************
  * @file    COMP/COMP2_FrequencyMeasurement/readme.txt 
  * @author  MCD Application Team
  * @version V1.5.2
  * @date    30-September-2014
  * @brief   Description of the COMP2 frequency and duty cycle measurement Example.
  ******************************************************************************
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
   @endverbatim

  @par Example description

  This example shows how to configure the COMP2 peripheral to measure external 
  signal frequency and duty cycle.
  This method (measuring signal frequency using comparator) is interesting when
  external signal doesn't respect the VIL and VIH levels.

  - COMP2 is configured as follow:
     - Inverting input is internally connected to DAC1 output
     - Non Inverting input is connected to PD0
     - Output is internally connected to TIM2 input capture 2

  - DAC1 is configured in 8-bit right alignment. DAC1 output = VREF * DOR / 256 
                                                              = 3.3 * 85 / 256 
                                                              = 1V.
    In this example, the frequency and duty cycle of an external signal is around 1V
    can be performed (e.g. high level = 1.1V and low level = 0.9V)  
    DAC1 is used as a digital trigger for measurement.

  - TIM2 is configured in PWM input mode and slave reset mode using TI2FP2 as
    trigger input.
    TIM2_CCR2 register contains the signal frequency
    TIM2_CCR1 contains the high level duration

  - External signal should be connected to PD0.

  The frequency and the duty cycle of the external signal are stored in SignalFrequency
  and SignalDutyCycle variables:
    SignalFrequency = TIM2CLK / TIM2_CCR2 in Hz,
    SignalDutyCycle = (TIM1_CCR1 * 100)/ (TIM1_CCR2)in %.

  @par Directory contents

  - COMP\COMP2_FrequencyMeasurement\main.c            Main file containing the "main" function
  - COMP\COMP2_FrequencyMeasurement\stm8l15x_conf.h   Library Configuration file
  - COMP\COMP2_FrequencyMeasurement\stm8l15x_it.c     Interrupt routines source
  - COMP\COMP2_FrequencyMeasurement\stm8l15x_it.h     Interrupt routines declaration


  @par Hardware and Software environment

    - This example runs on STM8L15x High-Density, Medium-Density Plus, and 
      Medium-Density Devices.

  - This example has been tested with STMicroelectronics STM8L1528-EVAL 
    (STM8L15x High-Density devices) and STM8L1526-EVAL (STM8L15x Medium-Density 
    devices) evaluation boards and can be easily tailored to any other development
    board.

  - STM8L1528-EVAL & STM8L1526-EVAL Setup
     - Make sure that the LCD glass daughter board is mounted in IO position.
       For more details please refer to the evaluation board user manual.

  @par How to use it ?

  In order to make the program work, you must do the following :

  - Copy all source files from this example folder to the template folder under
    Project\STM8L15x_StdPeriph_Template
  - Open your preferred toolchain 
  - Rebuild all files and load your image into target memory
  - Run the example
  - Connect the external signal to measure to PD0
  - Add "SignalFrequency" and "SignalDutyCycle" variables in watch to display 
    the frequency and duty cycle of the external signal.(declared in stm8l15x_it.c)



 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
