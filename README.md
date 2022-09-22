# X-CUBE-MEMSMIC1 Firmware Package

![latest tag](https://img.shields.io/github/v/tag/STMicroelectronics/x-cube-memsmic1.svg?color=brightgreen)

The **X-CUBE-MEMSMIC1** software package is an expansion for **STM32Cube**. 

This software provides drivers running on **STM32** for the acquisition of ST digital MEMS microphones (**MP34DT06J**) and ST analog MEMS microphones (**MP23ABS1**). 

It is built on top of **STM32Cube** software technology that ease portability across different STM32 microcontrollers.

X-CUBE-MEMSMIC1 also includes an example of ultrasound condition monitoring (**UltrasoundFFT**) that calculates the FFT of the analog microphone signal and streams the result to a PC GUI via USB.

The package also offers an example to exploit the Performance Mode of the **MP23DB01HP** MEMS audio sensor multi performance mode digital microphone available on the **STEVAL-MIC006V1** coupon board.

**X-CUBE-MEMSMIC1** software features:

- Complete middleware to build applications using ST MEMS digital and analog microphones.
- Easy portability across different MCU families thanks to STM32Cube.
- Sample applications that the developer can use to start experimenting with the code.
- References to free application that can be used to record and save acquired microphones audio signals.
- Free user-friendly license terms.
- Examples implementation available for the following STM32 Nucleo expansion board: 
    - **X-NUCLEO-CCA02M2** 
    - **X-NUCLEO-AMICAM1**
- The STM32 Nucleo expansion board can be plugged on top of
    - **NUCLEO-F401RE**
	- **NUCLEO-L476RG**
    - **NUCLEO-F746ZG**
	- **P-NUCLEO-WB55**
    - **NUCLEO-L4R5ZI**
- Examples implementation available for **STEVAL-STWINKT1B** and STEVAL-STWINKT1 SensorTile Wireless Industrial Node (STWIN) development kits and for the **STEVAL-STWINMAV1**
microphone array expansion board.

The figure below shows the overall architecture.

![](_htmresc/X-CUBE-MEMSMIC1_components.PNG)

- At the bottom layer there are the HW components: the STM32 MCU and the MP34DT06J digital MEMS microphone or the MP23ABS1 analog MEMS microphone.
- The drivers abstract low level details of the hardware and allow the middleware layer to provide functionalities 
  in a hardware independent fashion. In this particular case, middleware implements PDM to PCM decimation filter 
  and a custom USB Audio input class.
- The application provide examples of how to use the code.

Here is the list of references to user documents:

- [UM1901](https://www.st.com/resource/en/user_manual/dm00187405.pdf) : Getting started with the software package for digital MEMS microphones in X-CUBE-MEMSMIC1 expansion for STM32Cube
- [UM2214](https://www.st.com/resource/en/user_manual/dm00391112.pdf) : Getting started with Acoustic BF real time beamforming library
- [UM2212](https://www.st.com/resource/en/user_manual/dm00390468.pdf) : Getting started with Acoustic SL real-time sound source localization middleware
- [MP34DT06J](https://www.st.com/content/st_com/en/products/audio-ics/mems-microphones/mp34dt06j.html) : MEMS audio sensor omnidirectional digital microphone datasheet
- [MP23ABS1](https://www.st.com/content/st_com/en/products/audio-ics/mems-microphones/mp23abs1.html) : High performance MEMS audio sensor single ended analog bottom-port microphone
- [STM32Cube](https://www.st.com/stm32cube) : STM32Cube
- [STM32 Nucleo boards](https://www.st.com/stm32nucleo) : STM32 Nucleo boards
- [STM32 Nucleo expansion boards](https://www.st.com/x-nucleo) : STM32 Nucleo expansion boards

## Known Limitations

-  **High Performance Microphones Streaming** example uses the new **HP_PDMFilter** library and is compatible only with **STEVAL-MIC006V1**. The example is available on STM32F401 and STM32F746 for X-NUCLEO-CCA02M2.
-  Beamforming and Source localization examples are available only on STM32F401 for X-NUCLEO-CCA02M2 and on STM32L4+ for X-NUCLEO-AMICAM1. A Source localization example is also available on STWIN board.
-  To use X-NUCLEO-AMICAM1 safely, read carefully hardware configuration needed. See [UM1901](https://www.st.com/resource/en/user_manual/dm00187405.pdf) and [UM2649](https://www.st.com/resource/en/user_manual/dm00663665.pdf)

## Development Toolchains and Compilers

-   IAR Embedded Workbench for ARM (EWARM) toolchain V9.20.1
-   RealView Microcontroller Development Kit (MDK-ARM) toolchain V5.37.0
-   STM32CubeIDE Version 1.10.1

## Supported Devices and Boards

- STM32WB55, STM32F401, STM32L476, STM32L4R5 and STM32F746 devices
- [P-NUCLEO-WB55](https://www.st.com/content/st_com/en/products/evaluation-tools/product-evaluation-tools/stm32-nucleo-expansion-boards/p-nucleo-wb55.html)
- [NUCLEO-F401RE](https://www.st.com/content/st_com/en/products/evaluation-tools/product-evaluation-tools/mcu-mpu-eval-tools/stm32-mcu-mpu-eval-tools/stm32-nucleo-boards/nucleo-f401re.html) Rev C
- [NUCLEO-F746ZG](https://www.st.com/content/st_com/en/products/evaluation-tools/product-evaluation-tools/mcu-mpu-eval-tools/stm32-mcu-mpu-eval-tools/stm32-nucleo-boards/nucleo-f746zg.html) Rev B
- [NUCLEO-L476RG](https://www.st.com/content/st_com/en/products/evaluation-tools/product-evaluation-tools/mcu-mpu-eval-tools/stm32-mcu-mpu-eval-tools/stm32-nucleo-boards/nucleo-l476rg.html) Rev C
- [NUCLEO-L4R5ZI](https://www.st.com/content/st_com/en/products/evaluation-tools/product-evaluation-tools/mcu-mpu-eval-tools/stm32-mcu-mpu-eval-tools/stm32-nucleo-boards/nucleo-l4r5zi.html)
- [X-NUCLEO-CCA02M2](https://www.st.com/content/st_com/en/products/ecosystems/stm32-open-development-environment/stm32-nucleo-expansion-boards/stm32-ode-sense-hw/x-nucleo-cca02m2.html)
- [X-NUCLEO-AMICAM1](https://www.st.com/content/st_com/en/products/ecosystems/stm32-open-development-environment/stm32-nucleo-expansion-boards/stm32-ode-sense-hw/x-nucleo-amicam1.html)
- STEVAL-STWINKT1
- [STEVAL-STWINKT1B](https://www.st.com/en/evaluation-tools/steval-stwinkt1b.html)
- [STEVAL-STWINMAV1](https://www.st.com/en/evaluation-tools/steval-stwinmav1.html)

## Backward Compatibility

- X-NUCLEO-CCA02M1 is not supported anymore, but you can still use this outdated board running the X-NUCLEO-CCA02M2 + NUCLEO-F401RE, NUCLEO-F746ZG or NUCLEO-L476RG examples due to HW compatibility between the two shields.

## Dependencies

- None
