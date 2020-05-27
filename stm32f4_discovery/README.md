# STM32F4Discovery Examples

A video tutorials on how to build and run these examples:

[![Windows remcu F4 discvoery](img/youtube_preview_win.png)](https://youtu.be/iuPJtNPQ9iA)

[![Unix(Macos) remcu F4 discvoery](img/youtube_preview_mac.png)](https://youtu.be/iuofdoWWkC4)

## accell_graph
It is a GUI app which grabs acceleration data from on-board(STM32F4Discovery) MEMS sensor and plot graph of it vs. time in real time.  
<details>
  <summary>demo
<b>(click here)</b></summary>

![](accell_graph/img/mems_demo.gif)
</details>

## CAN_BUS
It is a GUI app of [CAN Bus](https://en.wikipedia.org/wiki/CAN_bus) sniffer based on STM32F4Discovery board.
<details>
  <summary>demo
<b>(click here)</b></summary>

![](CAN_BUS/img/can_bus_demo.gif)
</details>

## jupyter-notebook
There are scripts for [Jupyter Notebook](https://jupyter.org/) IDE. The Python script contains work with GPIO and Timer PWM. The C++ script (Cling Interpreter) contains works with GPIO, Timer, ADC, DMA, DAC modules.  
<details>
  <summary>demo
<b>(click here)</b></summary>

![slider_f4d](jupyter-notebook/img/py_demo.gif)  
</details>

## STM32F4-Discovery_FW_V1.1.0
[Standard Peripheral Library(SPL) for STM32F4DISCOVERY](https://www.st.com/content/st_com/en/products/embedded-software/mcu-mpu-embedded-software/stm32-embedded-software/stm32-standard-peripheral-library-expansion/stsw-stm32068.html#overview) with a description of SPL samples([AN3983](https://www.st.com/content/ccc/resource/technical/document/application_note/f1/6d/73/bb/50/a5/47/17/DM00038796.pdf/files/DM00038796.pdf/jcr:content/translations/en.DM00038796.pdf) doc). All the code controlling STM32F4 peripherals in examples above have been taken from there.

