# REMCU examples for STM32F030 and STM32F051 board

![raspberry remcu stm32f030](img/rasp_f030.png)

## Overview

These simple examples demonstrate how to easily expand hardware of a Raspberry Pi board using 50 cent STM32F030 microcontroller(and other low-cost STM32F0 MCUs) and some wires.  
The Python scripts ran Raspberry will gain access to internal STM32F0 peripheral modules like DAC, ADC, DMA, GPIO and others as if these peripherals were a part of Raspberry chip. Without a microcontroller firmware development and a kernel driver just with help [**REMCU Library**](https://remotemcu.com/).

There are also [Jupyter Notebook](https://jupyter.org/) scripts for running on PC(MacOS, Linux, Windows)

To interact with a microcontroller hardware from our PC and Raspberry Pi we use the driver functions of [Standard Peripheral Library for STM32F0 line](https://www.st.com/en/embedded-software/stsw-stm32048.html) of STMicroelectronics company. It is a vendor software development kit provided by STMicroelectronics company for a firmware development of STM32 MCU.
Our application calls these driver functions, just like calls them for hardware operations from firmware code. The REMCU Library translates a local driver function call on the PC or embedded computer to a remote call of the same​ driver function on the STM32F0 chip using debug interface.

The code controlling MCU peripheral module have been taken from  [example](STM32F0_StdPeriph_Lib_V3.5.0/Project/STM32F0_StdPeriph_Examples) of [Standard Peripheral Library for STM32F0](https://www.st.com/en/embedded-software/stsw-stm32048.html)

## Table of Contents
1. [Raspberry Pi](#1-raspberry-pi)
2. [MacOS/Linux](#2-macoslinux)
3. [Windows](#3-windows)

## 1. Raspberry Pi

A video tutorial on how to build and run the [similar examples](../STM32F103RCT6) on Raspberry Pi computer:

[![Raspberry](img/preview.png)](https://youtu.be/JdBabbC5Prk)

### Prerequirements
 * A Raspberry Pi 1/2/3/4 board
 * A board with the STM32F030 or stm32F051 device. There are demonstration on [STM32F030 DEMO BOARD V1.1](https://stm32-base.org/boards/STM32F030F4P6-STM32F030-DEMO-BOARD-V1.1.html) and [LC Technology STM32F051C8T6](https://stm32-base.org/boards/STM32F051C8T6-LC-Technology)
 * 4 wires to connect Pi with MCU
 * A specify [***image***](https://remotemcu.com//images/2020-06-03-raspberry-remcu.zip) of Raspberry containing these examples, prebuild OpenOCD utility and REMCU library  
  or install/download these components to your Raspbian filesystem manually. [Instructions](#manual-configuration-installation-all-necessary-components)

### Prepare
Download the [***image***](https://remotemcu.com//images/2020-06-03-raspberry-remcu.zip) and unpack the archive. Write the image to your SD card using the official Raspberry Pi [documentation](https://www.raspberrypi.org/documentation/installation/).  
[![flash image](https://img.youtube.com/vi/CCRVo5sI1E0/0.jpg)](https://www.youtube.com/watch?v=CCRVo5sI1E0)  
Insert the card into the board and switch Raspberry Pi on.  Connect a keyboard and a monitor or connect using SSH. By default, the system has a zeroconf mDNS server(Avahi), you can connect Raspbery Pi using ***remcu.local*** name
```bash
ssh pi@remcu.local
```
If you plug Raspberry Pi into laptop/PC directly without a DHCP service, the Rpi will has a ***static IP address 192.168.0.10*** (255.255.255.0 mask and 192.168.0.1 gateway).  
The default login is ‘pi’ and the default password is ‘raspberry’. Home directory has everything necessary to build and run the examples.  
This image has a community version of the REMCU lib which works through debug interface and requires an OpenOCD utility. Connect this Raspberry GPIO pins to debug port of the MCU using scheme below.  
<details>
  <summary>scheme for Raspberry Pi 1
<b>(click here) </b></summary>

![scheme](img/scheme-51.png)
![scheme](img/scheme-30.png)
</details>

<details>
  <summary>scheme for Raspberry Pi 2/3/4
<b>(click here) </b></summary>

![scheme](img/scheme-51_2_3_4.png)
![scheme](img/scheme-30_2_3_4.png)
</details>

Run the “openocd_stm32f0x.sh” scripts:  
<details>
  <summary> OpenOCD result
<b>(click here) </b></summary>

![openocd](img/run_openocd_stm32f0x.sh.png)
</details>

Navigate to ‘examples_stm32f030’ or ‘examples_stm32f051’ directory depending on your board.

#### Manual configuration. Installation all necessary components. For 32-bit Raspbian System
That requires next:  
1. Downlaod prebuild OpenOCD from [here](https://github.com/remotemcu/Raspbian_packages) to your Raspbian RootFS. The build have been tested on Raspberry Pi and we recommend it.  

```bash
git clone https://github.com/remotemcu/Raspbian_packages.git
```

Connect this Raspberry's GPIO pins to debug port of the MCU using scheme above and run the OpenOCD script:

```bash
cd Raspbian_packages/
./openocd_stm32f0x.sh
```

Another way you can install OpenOCD(version 0.10 or higher) from Raspbian's repo:  

```bash
sudo apt-get install openocd
```
In this case, run OpenOCD utility this way:
* For Raspberry 1, use this scheme [stm32f030](img/scheme-30_2_3_4.png), [stm32f051](img/scheme-51_2_3_4.png) (SWDIO -> GPIO25 and SWCLK -> GPIO11) and run OpenOCD with the arguments below:
```bash
sudo openocd -f interface/raspberrypi-native.cfg -c "transport select swd" -f target/stm32f0x.cfg
```
* For Raspberry 2 or 3:
```bash
sudo openocd -f interface/raspberrypi2-native.cfg -c "transport select swd" -f target/stm32f0x.cfg
```
* For Raspberry 4, please use sysfsgpio interface:
```bash
sudo openocd -f interface/sysfsgpio-raspberrypi.cfg -c "transport select swd" -f target/stm32f0x.cfg
```
or try to use our [prebuilt OpenOCD package](https://github.com/remotemcu/Raspbian_packages) with the **openocd_stm32f0x.sh** script

>More information about Rpi connection with MCU using OpenOCD can be found here:
https://learn.adafruit.com/programming-microcontrollers-using-openocd-on-raspberry-pi
or
https://iosoft.blog/2019/01/28/raspberry-pi-openocd/

2. Download these examples to Raspberry RootFS and go to STM32F030-51 directory  

```bash
 git clone https://github.com/remotemcu/remcu_examples/
 cd STM32F030-51/
```

3. Download REMCU Lib archive from [**download page**](https://remotemcu.com/download):  
Target - **STM32F030XX** or **STM32F051**
Library - **StdPeriph_Lib**  
Version - **V1.5.0**  
Operation system - **Raspbian**

<details>
  <summary>download page
  <b>(click here) </b></summary>
  
![download lib](img/download_unix.png)
</details>

4. Extract it to the example folder  

```bash
$pwd 
.../STM32F030-51/
$tar -xf remcu-*.tar
```


### Python scripts

You can use the ReMCU library not only in the C/C++ program on the Raspberry Pi board. Alternatively, you can call ReMCU library functions using other programming languages. As Python is an official recommended programming language of Raspberry, let’s see some Python examples. To make working with them less troublesome, we provide language wrappers(**STM32F030_StdPeriph_Lib.py** and **STM32F051_StdPeriph_Lib.py** in the REMCU archive).
 With these wrappers it becomes easy to work with hardware as it is using C or C++.  
 <details>
  <summary>
Examples: <b>Python and C/C++(click here) </b></summary>

![diff lang](../img/diff.png)
</details>
  
  * [**PA4_pin_toogle.py**](PA4_pin_toogle.py) and [**PC13_pin_toogle.py**](PC13_pin_toogle.py) script toggles the PA4 and PC13 MCU pin accordingly. The code of the sample have been taken from **GPIO_IOToggle** ([main.c](STM32F0xx_StdPeriph_Examples/GPIO/GPIO_IOToggle/main.c)) example of [Standard Peripheral Library(SPL) for STM32F0x line](https://www.st.com/en/embedded-software/stsw-stm32048.html) Just execute it:  
  
```bash
python PA4_pin_toogle.py
```

```bash
python PC13_pin_toogle.py
```

![led](img/led-crop.gif)  

A small script [**adc_line.py**](adc_line.py) receives the ADC data and builds a horizontal bar chart  based on this value. Plug a variable resistor into the PA5 pin and run the application. The code of the sample is based on **ADC_BasicExample** ([main.c](STM32F0xx_StdPeriph_Examples/ADC/ADC_BasicExample/main.c)) example of [Standard Peripheral Library(SPL) for STM32F0x line](https://www.st.com/en/embedded-software/stsw-stm32048.html)  
Rotate the shaft and you will get the voltage versus time chart.  
![adc_line](img/adc_console.gif)  

> Note that we didn’t develop the MCU firmware. Our Linux program directly works with ADC and GPIO modules using a remote procedure execution of the Standard Peripheral Library. In more detail in [video](https://youtu.be/JdBabbC5Prk)   


The REMCU library can be ported to any version of the Raspberry Pi board and also on far less powerful and much more powerful embedded platforms  
![other_plats](img/other_plats.png)

## 2. MacOS/Linux
### Prerequirements
 * A board with the STM32F030 or stm32F051 device. There are demonstration on [STM32F030 DEMO BOARD V1.1](https://stm32-base.org/boards/STM32F030F4P6-STM32F030-DEMO-BOARD-V1.1.html) and [LC Technology STM32F051C8T6](https://stm32-base.org/boards/STM32F051C8T6-LC-Technology)
 * A debugger like st-link or jlink or others
 * Debug server. It can be an OpenOCD([prebuild package](https://github.com/ilg-archived/openocd/releases/tag/v0.10.0-12-20190422)) or st-link GDB server from [Atollic TrueStudio](https://atollic.com/truestudio/) or [Jlink GDB server](https://www.segger.com/products/debug-probes/j-link/tools/j-link-gdb-server/about-j-link-gdb-server/) (if you use jlink instead of st-link)
 * [Anaconda Distribution](https://www.anaconda.com/distribution/), ***we recommend Python 2 version***

### Prepare
Download the example. Download the necessary version of REMCU Lib from [**download page**](https://remotemcu.com/download):  
Target - **STM32F030XX** or **STM32F051**
Library - **StdPeriph_Lib**  
Version - **V1.5.0**  
Operation system - **MacOS/Linux**

<details>
  <summary>download page
	<b>(click here) </b></summary>
	
![download lib](img/download_unix.png)
</details>

Extract the library to this folder.  

Connect the STM32F0 board with debugger to PC:  
![connection](img/connection.png)

Run OpenOCD with arguments above:

  + st-link debugger
```bash 
openocd -f interface/stlink-v2.cfg -f target/stm32f0x.cfg
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
Run Jupyter Notebook and open the *Cpp_Examples.ipynb* file in it. The script contains detailed comments for work. You just do step by step code of the script and observe the results.  
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
* A board with the STM32F030 or stm32F051 device. There are demonstration on [STM32F030 DEMO BOARD V1.1](https://stm32-base.org/boards/STM32F030F4P6-STM32F030-DEMO-BOARD-V1.1.html) and [LC Technology STM32F051C8T6](https://stm32-base.org/boards/STM32F051C8T6-LC-Technology)
 * A debugger like st-link or jlink or others
 * Debug server. It can be an OpenOCD([prebuild package](https://github.com/ilg-archived/openocd/releases/tag/v0.10.0-12-20190422)) or st-link GDB server from [Atollic TrueStudio](https://atollic.com/truestudio/) or [Jlink GDB server](https://www.segger.com/products/debug-probes/j-link/tools/j-link-gdb-server/about-j-link-gdb-server/) (if you use jlink instead of st-link)
 * [Anaconda Distribution](https://www.anaconda.com/distribution/), ***we recommend Python 2 version***

### Prepare
Download the example. Download the necessary version of REMCU Lib from [**download page**](https://remotemcu.com/download):  
Target - **STM32F030XX** or **STM32F051**
Library - **StdPeriph_Lib**  
Version - **V1.5.0**  
Operation system - **Windows**
<details>
  <summary>download page
	<b>(click here) </b></summary>
	
![download lib](img/download_unix.png)
</details>

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
Run Jupyter Notebook and open the Python_Examples.ipynb file in it.  
The script contains detailed comments for work. You just do step by step code of the script and observe the results.  
![run py script](img/f103_adc.gif)




