## __Beamforming application__

This document provides a description of the sound source localization application based 
on Nucleo boards and X-NUCLEO-AMICAM1 expansion board. It allows acquisition of 
2 digital MEMS microphones that are used to run beamforming library in order to  
create a directional virtual microphone. The audio is then streamed via the 
X-NUCLEO-AMICAM1 usb port to an host PC running a standard audio editing tool.

NOTE: to fully take advantage of this example, only the two on-board microphone are used.
Solder bridge configuration must be set for 2 microphone acquisition, as depicted in the 
X-CUBE-MEMSMIC1 user manual.

NOTE: the example works only using a sampling frequency of 16KHz. To make the on-board ADC
working properly, you must close the soldier bridge SB11 to pass to the adau1978 component
an extra signal, the I2S master clock. 

A analog MEMS microphone can be acquired by using different peripherals, 
such as  I²S, SPI or DFSDM, using on-board external ADC.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals. 
Then the SystemClock_Config() function is used to configure the system clock (SYSCLK). 
The resulting system clock speed depends on the MCU used and it's equal to 80MHz for 
STM32L4

At this point the following steps are required:

- USB Audio In configuration:
At first the USB descriptor is set up according to the number of channels 
to be streamed and the desired frequency. This allows the device to be 
recognized as a standard USB microphone with the requested configuration. 
Then the firmware initializes USB core and start USB functionalities 
with the standard USB Device functions.

- Audio peripheral configuration and start of the acquisition: 
Using BSP layer and Middlewares functionalities, all the required peripherals and 
libraries are configured, basing on the number of channels to be streamed and the 
desired sampling frequency to be achieved. These steps start inside the 
usbd_audio_if.c file, whose functions are called in response to the USB enumeration 
that starts when the device is connected to the PC. 
An alternative approach would be to manage the configuration and starting of the 
audio peripherals by calling directly the BSP functionalities from the application
space.
In the firmware, audio-related parts, concerning action to be performed in response 
to DMA callbacks, are collected in the audio_application.c 

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
The application need to ensure that the SysTick time base is always set to 1 millisecond
to have correct HAL operation.

### __Directory contents__

  - Acoustic_BF/Src/stm32xx_it.c            	Interrupt handlers file for STM32F4
  - Acoustic_BF/Src/system_stm32f4xx.c      	STM32F4xx system clock configuration file
  - Acoustic_BF/Src/usbd_audio_if.c		    	USBD Audio-input interface  
  - Acoustic_BF/Src/usbd_conf_f4.c    			USB device driver Configuration file for STM32F4
  - Acoustic_BF/Src/usbd_desc.c    				USB device AUDIO-input descriptor   
  - Acoustic_BF/Src/cube_hal_f4.c           	Clock configuration file for STM32F4
  - Acoustic_BF/Src/main.c                  	Main program	
  - Acoustic_BF/Src/audio_application.c  		Audio application file
	
  - Acoustic_BF/Inc/stm32f4xx_hal_conf.h    	HAL configuration file for STM32F4
  - Acoustic_BF/Inc/stm32xx_it.h            	Interrupt handlers header file for STM32F4
  - Acoustic_BF/Inc/main.h                  	Main program header file
  - Acoustic_BF/Inc/cube_hal.h			    	X-CUBE includes file
  - Acoustic_BF/Inc/audio_application.h  		Application header file
  - Acoustic_BF/Inc/usbd_audio_if.h		    	USBD Audio-input interface header file  
  - Acoustic_BF/Inc/usbd_conf.h    				USB device driver Configuration file
  - Acoustic_BF/Inc/usbd_desc.h    				USB device AUDIO-input descriptor header file
  - Acoustic_BF/Inc/cca02m2_conf.h 	X-NUCLEO-CCA02M2 configuration file
  - Acoustic_BF/Inc/cca02m2_errno.h	Error file  


### __Hardware and Software environment__

  - This example runs on STM32L4R5ZI devices.
    
  - This example has been tested with STMicroelectronics NUCLEO-L4R5ZI
    boards and can be easily tailored to any other supported device 
    and development board.
    

### __How to use it ?__

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example
 - In order to stream audio via USB, connect X-NUCLEO-AMICAM1 USB connector to a host PC

