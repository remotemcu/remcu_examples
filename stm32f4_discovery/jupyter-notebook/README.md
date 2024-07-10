# Jupyter Notebook Examples

## Overview
If you ever wish to conduct a quick experiment with any chip peripheral module or explore it, then [**REMCU Lib**](https://remotemcu.github.io/) is the right tool for that. It can be easily integrated in [Jupyter Notebook](https://jupyter.org/) with an Interactive C++ interpreter . This IDE is free and it lets you run and execute C or C++ code on the go. This way you can call the functions of a driver directly and control MCU or SoC in real time.  
It saves a lot of time that is usually spent on the development of experimental MCU firmware. An interact work with MCU/SoC peripherals helps us to explore new peripheral blocks of MCU/SoC quicker.  
And furthermore, the application code written in the C programming language using REMCU is backwards-portable, meaning it can be transferred from a desktop program to the MCU program and vice-versa. The code will work just as fine on MCU as it works on PC. That's why code written on desktop with ReMCU can be used to develop stand-alone firmware for MCU.  
There is also possible to use the REMCU with high level languages like Python, Java, C# and others.  To make working with them less troublesome, we provide language wrappers. With these wrappers it becomes easy to work with hardware as it is using C or C++.

![Jupiter_static](img/Jupiter_static.png)

*These examples demonstrates how to easily use REMCU Lib with C++ ([Cling](https://github.com/root-project/cling)) interpreter and Python interpreter*


## Prerequirements
  * An [STM32F4DISCOVERY](https://www.st.com/en/evaluation-tools/stm32f4discovery.html) board
  * PC with MacOS or Linux(Ubuntu recommended) or Windows OS
  * An oscilloscope to watch DAC signal. It is optional.
  * [Anaconda Distribution](https://www.anaconda.com/distribution/), ***we recommend Python 2 version***
  * A debug server. It can be an OpenOCD(**We recommend this [prebuild package](https://github.com/ilg-archived/openocd/releases/tag/v0.10.0-12-20190422)**)
>Other OpenOCD versions can have an incompatibility with this REMCU Library version. For instance, [v0.10.0-13](https://github.com/xpack-dev-tools/openocd-xpack/releases/tag/v0.10.0-13) version has another TCL server API and you can not use remcu_connect2OpenOCD function in REMCU Library for connecting to OpenOCD TCL server(default 6666 port). Only remcu_connect2GDB function for connecting OpenOCD GDB server (default 3333 port).

## Prepare

Install the [Anaconda Distribution](https://www.anaconda.com/distribution/) 

Download the example. Download the necessary version of REMCU Lib from [**repo**](https://github.com/remotemcu/prebuilt_libraries/tree/master/STM32/STM32F4/STM32F4XX-StdPeriph_Lib-V1.1.0-01) or build the library yourself using [the sdk collection repo ](https://github.com/remotemcu/remcu-chip-sdks) 

Copy the lib files and *remcu_include* folder to this folder.
<details>
  <summary>show <b>(click here)</b></summary>
  
![extract.png](img/extract.png)
</details>

Connect the Discovery board to your PC, then

## C++ script
C++ [Cling](https://github.com/root-project/cling) interpreter is only available for Unix-like system (***MacOS*** and ***Linux***). Run Anaconda Navigator and install [**xeus-cling**](https://github.com/jupyter-xeus/xeus-cling), [**xwidgets**](https://github.com/jupyter-xeus/xwidgets) and [**xplot**](https://github.com/QuantStack/xplot) packages.  
<details>
  <summary>xeus install show
<b>(click here) </b></summary>

![](img/xeus.png)
![](img/xwidgets.png)
![](img/xplot.png)
</details>

Run Jupyter Notebook and open the stm32f4_discovery.ipynb file in it. The script contains detailed comments for work. You just do step by step code of the script and observe the results.  


Description of REMCU functions can be found can be found in [API reference](https://remotemcu.github.io/api-v1-0)  
Code controlling MCU peripherals have been taken from examples of [Standard Peripheral Library for STM32F4DISCOVERY](https://www.st.com/content/st_com/en/products/embedded-software/mcu-mpu-embedded-software/stm32-embedded-software/stm32-standard-peripheral-library-expansion/stsw-stm32068.html#overview). You can see description of their at [AN3983](https://www.st.com/content/ccc/resource/technical/document/application_note/f1/6d/73/bb/50/a5/47/17/DM00038796.pdf/files/DM00038796.pdf/jcr:content/translations/en.DM00038796.pdf) doc.


## Python script
Install [**ipywidgets**](https://ipywidgets.readthedocs.io/en/stable/user_install.html) package.  
***We encourage you use Python 2 version***. We can not guarantee the work of the script with Python 3.  
Run Jupyter Notebook and open the stm32f4_discovery_python.ipynb file in it. The script contains detailed comments for work. You just do step by step code of the script and observe the results.  
<details>
  <summary> Py script show
<b>(click here) </b></summary>

![slider_f4d](img/py_demo.gif)
</details>

For the operation convenience, we prepared a particular Python wrapper(STM32F4XX_StdPeriph_Lib.py in remcu_include folder) containing all necessary constants and structures which makes it possible to use the Standard Peripheral Library function just like with C and C++ code.  
<details>
  <summary>Example <b>Python - C/C++</b></summary>
  
![diff](../../img/diff.png)
</details>

Description of REMCU functions can be found can be found in [API reference](https://remotemcu.github.io/api-v1-0)  
Code controlling MCU peripherals have been taken from examples of [Standard Peripheral Library for STM32F4DISCOVERY](https://www.st.com/content/st_com/en/products/embedded-software/mcu-mpu-embedded-software/stm32-embedded-software/stm32-standard-peripheral-library-expansion/stsw-stm32068.html#overview). You can see description of their examples at [AN3983](https://www.st.com/content/ccc/resource/technical/document/application_note/f1/6d/73/bb/50/a5/47/17/DM00038796.pdf/files/DM00038796.pdf/jcr:content/translations/en.DM00038796.pdf) doc.
























