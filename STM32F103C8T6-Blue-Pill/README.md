# REMCU examples for STM32F103C8T6 Blue Pill board

![title](img/title.png)

## Overview

These simple examples demonstrate how to easily expand hardware of a Raspberry Pi board using one dollar STM32F103C8T6 microcontroller and some wires.  
The C++ applications and Python scripts ran Raspberry will gain access to internal STM32F103 peripheral modules like DAC, ADC, DMA, GPIO and others as if these peripherals were a part of Raspberry chip. Without a microcontroller firmware development and a kernel driver just with help [**REMCU Library**](https://remotemcu.com/).

There are also [Jupyter Notebook](https://jupyter.org/) scripts for running on PC(MacOS, Linux, Windows)

To interact with a microcontroller hardware from our PC we use the driver functions of [Standard Peripheral Library for STM32F10x line](https://www.st.com/content/st_com/en/products/embedded-software/mcu-mpu-embedded-software/stm32-embedded-software/stm32-standard-peripheral-libraries/stsw-stm32054.html) of STMicroelectronics company. It is a vendor software development kit provided by STMicroelectronics company for a firmware development of STM32F103C8T6 MCU.
Our application calls these driver functions, just like calls them for hardware operations from firmware code.

The code controlling MCU peripheral module have been taken from  [example](STM32F10x_StdPeriph_Lib_V3.5.0/Project/STM32F10x_StdPeriph_Examples) of [Standard Peripheral Library for STM32F10X](https://www.st.com/content/st_com/en/products/embedded-software/mcu-mpu-embedded-software/stm32-embedded-software/stm32-standard-peripheral-libraries/stsw-stm32054.html)

## Table of Contents
1. [Raspberry Pi](#1-raspberry-pi)
   + [C++ programs](#C++-programs)
   + [Python scripts](#Python-scripts)
2. [MacOS/Linux](#2-macoslinux)
3. [Windows](#3-windows)

## 1. Raspberry Pi

A video tutorial on how to build and run the [similar examples](../STM32F103RCT6) on Raspberry Pi computer:

[![Raspberry](img/preview.png)](https://youtu.be/JdBabbC5Prk)

### Prerequirements
 * A Raspberry Pi 1/2/3/4 board
 * A [Blue Pill board](https://stm32-base.org/boards/STM32F103C8T6-Blue-Pill.html) with original(noncounterfeit/no clones) STM32F103C8T6 MCU
 * 4 wires to connect Pi with MCU
 * A specify [***image***](https://remotemcu.com//images/2020-06-03-raspberry-remcu.zip) of Raspberry containing these examples, prebuild OpenOCD utility and REMCU library.  
 or install/download these components to your Raspbian filesystem manually. [Instructions](#manual-configuration-installation-all-necessary-components)

### Prepare
#### Using the image
Download the [***image***](https://remotemcu.com//images/2020-06-03-raspberry-remcu.zip) and unpack the archive. Write the image to your SD card using the official Raspberry Pi [documentation](https://www.raspberrypi.org/documentation/installation/).  
[![flash image](https://img.youtube.com/vi/CCRVo5sI1E0/0.jpg)](https://www.youtube.com/watch?v=CCRVo5sI1E0)  
Insert the card into the board and switch Raspberry Pi on.  Connect a keyboard and a monitor or connect using SSH. By default, the system has a zeroconf mDNS server(Avahi), you can connect Raspbery Pi using ***remcu.local*** name
```bash
ssh pi@remcu.local
```
<details>
  <summary>putty
<b>(click here to see) </b></summary>

![scheme](../img/putty_remcu.local.png)
</details>

If you plug Raspberry Pi into laptop/PC directly without a DHCP service, the Rpi will has a ***static IP address 192.168.0.10*** (255.255.255.0 mask and 192.168.0.1 gateway).  
The default login is ‘pi’ and the default password is ‘raspberry’. Home directory has everything necessary to build and run the examples.  
This image has a community version of the REMCU lib which works through debug interface and requires an OpenOCD utility. Connect this Raspberry GPIO pins to debug port of the MCU using scheme below.  
<details>
  <summary>scheme
<b>(click here to see) </b></summary>

![scheme](img/scheme.png)
![scheme](img/scheme_2_3_4.png)
</details>


Run the “openocd.sh” scripts:  
<details>
  <summary> OpenOCD result
<b>(click here) </b></summary>

![openocd_py](img/openocd_py.png)
</details>


Navigate to ‘STM32F103C8T6-Blue-Pill’ directory and run ‘make’ command. Several applications were created.  
<details>
  <summary> "make" result
<b>(click here) </b></summary>

![make](img/make.png)
</details>

#### Manual configuration. Installation all necessary components. For 32-bit Raspbian System
That requires next:  
1. Downlaod prebuild OpenOCD from [here](https://github.com/remotemcu/Raspbian_packages) to your Raspbian RootFS. The build have been tested on Raspberry Pi and we recommend it.

```bash
git clone https://github.com/remotemcu/Raspbian_packages.git
```

Connect this Raspberry's GPIO pins to debug port of the MCU using scheme above [1](img/scheme.png), [2/3/4](img/scheme_2_3_4.png) and run the OpenOCD script:

```bash
cd Raspbian_packages/
./openocd.sh
```

Another way you can install OpenOCD(version 0.10 or higher) from Raspbian's repo:  

```bash
sudo apt-get install openocd
```

In this case, run OpenOCD utility this way:  


* For Raspberry 1, use [this scheme](img/scheme_2_3_4.png) (SWDIO -> GPIO25 and SWCLK -> GPIO11) and run OpenOCD with the arguments below:
```bash
sudo openocd -f interface/raspberrypi-native.cfg -c "transport select swd" -f target/stm32f1x.cfg
```
* For Raspberry 2 or 3:
```bash
sudo openocd -f interface/raspberrypi2-native.cfg -c "transport select swd" -f target/stm32f1x.cfg
```
* For Raspberry 4, please use sysfsgpio interface:
```bash
sudo openocd -f interface/sysfsgpio-raspberrypi.cfg -c "transport select swd" -f target/stm32f1x.cfg
```
or try to use our [prebuilt OpenOCD package](https://github.com/remotemcu/Raspbian_packages) with the **openocd.sh** script

>More information about Rpi connection with MCU using OpenOCD can be found here:
https://learn.adafruit.com/programming-microcontrollers-using-openocd-on-raspberry-pi
or
https://iosoft.blog/2019/01/28/raspberry-pi-openocd/

2. Download these examples to Raspberry RootFS and go to STM32F103C8T6-Blue-Pill directory

```bash
 git clone https://github.com/remotemcu/remcu_examples/
 cd STM32F103C8T6-Blue-Pill/
```

3. Download REMCU Lib archive from [**download page**](https://remotemcu.com/download):  
Target - **STM32F10X_MD**  
Library - **StdPeriph_Lib**  
Version - **V3.5.0**  
Operation system - **Raspbian**

<details>
  <summary>download page
  <b>(click here) </b></summary>
  
![download lib](img/download103_mac.gif) 
</details>

4. Extract it to the example folder

```bash
$pwd 
..../STM32F103C8T6-Blue-Pill/
$tar -xf remcu-*.tar
```


### C++ programs

1. A small application ***ADC_line*** receives the ADC data and builds a horizontal bar chart  based on this value. Plug a variable resistor into the PA4 pin and run the application.  
<details>
  <summary> show 
<b>(click here) </b></summary>

![potentiometer](img/pot.png)
![var_resistor_scheme](img/var_resistor_scheme.png)
</details>

Rotate the shaft and you will get the voltage versus time chart.  
<details>
  <summary>
<b>show(click here) </b></summary>

![adc_line](img/rasp_adc.gif)  
</details>

2. There are two examples to work with CAN bus, ***CAN_send*** and ***CAN_receiver*** programs. These examples work on 125 kBps. For other baud rates and this clock configuration(see system_stm32f10x.c), the CAN Initialization code (Prescaler value) should be amended accordingly:  
```cpp
  //CAN_InitStructure.CAN_Prescaler = 4*16; //Prescaler for 62.5 kBps
  CAN_InitStructure.CAN_Prescaler = 4*8; //Prescaler for 125 kBps
  //CAN_InitStructure.CAN_Prescaler = 4*4; //Prescaler for 250 kBps
  //CAN_InitStructure.CAN_Prescaler = 4*2; //Prescaler for 500 kBps
  //CAN_InitStructure.CAN_Prescaler = 4*1; //Prescaler for 1 MBps
```
Build CAN network like this:  
<details><summary>
show(<b>click here</b>) </summary>

![](img/CAN_pic.jpg)

![](img/can_scheme.png)

</details>

* The ***CAN_receiver*** is a simple app that captures all CAN bus messages on the bus. Just run *CAN_receiver* app and look at the results:  
<details><summary>
show(<b>click here</b>) </summary>

![](img/can_demo_receiver.gif)
</details>

* The ***CAN_send*** is a simple app that send one message obtained through the program arguments. Example of using:  
<details><summary>
show(<b>click here</b>) </summary>

![](img/can_demo_send.gif)
</details>

> Note that we didn’t develop the MCU firmware. Our Linux program directly works with ADC and CAN modules using a remote procedure execution of the Standard Peripheral Library.  
<details><summary>
show(<b>click here</b>) </summary>

![spl](img/SPL.png)
</details>

### Python scripts

You can use the ReMCU library not only in the C/C++ program on the Raspberry Pi board. Alternatively, you can call ReMCU library functions using other programming languages. As Python is an official recommended programming language of Raspberry, let’s see some Python examples. To make working with them less troublesome, we provide language wrappers(**STM32F10X_MD_StdPeriph_Lib.py**).
 With these wrappers it becomes easy to work with hardware as it is using C or C++.  
 <details>
  <summary>
Examples: <b>Python and C/C++(click here) </b></summary>

![diff lang](../img/diff.png)
</details>
  

  * **gpio .py** script  toggles the PC13 MCU pin. Just execute it:  
<details><summary>
Show <b>(click here) </b></summary>

![gpio.py](img/rasp_gpio.gif)
</details>
  
```bash
python gpio.py
```
  * **ADC_line.py** script builds a horizontal bar chart of the ADC value versus time like the *C* ADC_line application.
<details>
  <summary>
	show <b>(click here) </b></summary>

![adc_line](img/rasp_adc.gif)  
</details>


The ReMCU library can be ported to any version of the Raspberry Pi board and also on far less powerful and much more powerful embedded platforms  
![other_plats](img/other_plats.png)

## 2. MacOS/Linux
### Prerequirements
 * A [Blue Pill board](https://stm32-base.org/boards/STM32F103C8T6-Blue-Pill.html) with original(noncounterfeit/no clones) STM32F103C8T6 MCU
 * A debugger like st-link or jlink
 * Debug server. It can be an OpenOCD(**We recommend this [prebuild package](https://github.com/ilg-archived/openocd/releases/tag/v0.10.0-12-20190422)**) or st-link GDB server from [Atollic TrueStudio](https://atollic.com/truestudio/) or [Jlink GDB server](https://www.segger.com/products/debug-probes/j-link/tools/j-link-gdb-server/about-j-link-gdb-server/) (if you use jlink instead of st-link)
 * [Anaconda Distribution](https://www.anaconda.com/distribution/), ***we recommend Python 2 version***

### Prepare
Download the example. Download the necessary version of REMCU Lib from [**download page**](https://remotemcu.com/download):  
Target - **STM32F10X_MD**  
Library - **StdPeriph_Lib**  
Version - **V3.5.0**  
Operation system - **MacOS/Linux**

![download lib](img/download103_mac.gif)  
Extract the library to this folder.  
Build examples, just  ***make*** command:  
<details>
  <summary> show
	<b>(click here) </b></summary>

![make_linux](img/make.png)  
</details>

Connect the STM32 board with debugger to PC:  
![connection](img/connection.png)

Run OpenOCD with arguments above:

  + st-link debugger
```bash 
./openocd -f interface/stlink-v2.cfg -f target/stm32f1x.cfg
```
<details>
  <summary>Success result for openocd
	<b>(click here) </b></summary>

![stlink_linux](img/openocd_unix.png)
</details>

#### Running...
There are the same programs and python scripts as above in the Raspberry part. You can see their description there.

#### Jupyter Notebook
If you ever wish to conduct a quick experiment with any chip peripheral module or explore it, then REMCU is the right tool for that. It can be easily integrated in [Jupyter Notebook](https://jupyter.org/) with an Interactive [C++ interpreter](https://github.com/root-project/cling) . This IDE is free and it lets you run and execute C or C++ code on the go. This way you can call the functions of a driver directly and control MCU or SoC in real time.  
It saves a lot of time that is usually spent on the development of experimental MCU firmware. An interact work with MCU/SoC peripherals helps us to explore new peripheral blocks of MCU/SoC quicker.  
And furthermore, the application code written in the C programming language using REMCU is backwards-portable, meaning it can be transferred from a desktop program to the MCU program and vice-versa. The code will work just as fine on MCU as it works on PC. That's why code written on desktop with ReMCU can be used to develop stand-alone firmware for MCU.  

[**REMCU tutorial**](https://youtu.be/iuofdoWWkC4) can help you run these notebooks.

 * **Cpp_Examples.ipynb** is a C++ script for Jupyter Notebook.  

Install [**Anaconda Distribution**](https://www.anaconda.com/distribution/). Run Anaconda Navigator and install [**xeus-cling**](https://github.com/jupyter-xeus/xeus-cling) and [**xplot**](https://github.com/QuantStack/xplot) packages.  
<details>
  <summary> Installed packages
	<b>(click here) </b></summary>

![](../stm32f4_discovery/jupyter-notebook/img/xeus.png)
![](../stm32f4_discovery/jupyter-notebook/img/xwidgets.png)
![](../stm32f4_discovery/jupyter-notebook/img/xplot.png)
</details>
Run Jupyter Notebook and open the "Cpp_Examples.ipynb" file in it. The script contains detailed comments for work. You just do step by step code of the script and observe the results.  
<details>
  <summary>
	ADC graph<b>(click here)</b></summary>
	
![run C++ script](img/adc_cpp.png)  
</details>

## 3. Windows
Unfortunately, C++ [Cling](https://github.com/root-project/cling) interpreter is only available for Unix-like system (***MacOS*** and ***Linux***).  
But there is possible to use the REMCU with high level languages like Python, Java, C# and others.

***Python_Examples.ipynb*** sample demonstrates work with REMCU lib in Windows using Python interpreter.

[**REMCU tutorial**](https://youtu.be/iuPJtNPQ9iA) can help you run these notebooks.

### Prerequirements
 * A [Blue Pill board](https://stm32-base.org/boards/STM32F103C8T6-Blue-Pill.html) with original(noncounterfeit/no clones) STM32F103C8T6 MCU
 * A debugger like st-link or jlink
 * Debug server. It can be an OpenOCD(**We recommend this [prebuild package](https://github.com/ilg-archived/openocd/releases/tag/v0.10.0-12-20190422)**) or st-link GDB server from [Atollic TrueStudio](https://atollic.com/truestudio/) or [Jlink GDB server](https://www.segger.com/products/debug-probes/j-link/tools/j-link-gdb-server/about-j-link-gdb-server/) (if you use jlink instead of st-link)
 * [Anaconda Distribution](https://www.anaconda.com/distribution/), ***we recommend Python 2 version***

### Prepare
Download the example. Download the necessary version of REMCU Lib from [**download page**](https://remotemcu.com/download):  
Target - **STM32F10X_MD**  
Library - **StdPeriph_Lib**  
Version - **V3.5.0**  
Operation system - **Windows**

![download lib](img/download103_mac.gif)  
Extract the library to this folder.  

Connect the STM32 board with debugger to PC:  
<details>
  <summary>diagram
	<b>(click here) </b></summary>

![connection](img/connection.png)
</details>
Run OpenOCD with arguments above:

  + st-link debugger
```bash 
openocd.exe -f interface/stlink-v2.cfg -f target/stm32f1x.cfg
```
![stlink_windows](img/openocd.png)

Install the [Anaconda Distribution](https://www.anaconda.com/distribution/) and [ipywidgets](https://ipywidgets.readthedocs.io/en/stable/user_install.html) package. ***We encourage you use Python 2 version***. We can not guarantee the work of the script with Python 3.  

Run Jupyter Notebook and open the ***Python_Examples.ipynb*** file in it.  
The script contains detailed comments for work. You just do step by step code of the script and observe the results.  
![run py script](img/f103_adc.gif)




