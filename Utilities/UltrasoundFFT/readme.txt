/**
@page UltrasoundFFT application for STWIN

@verbatim
  ******************************************************************************
  * @file    readme.txt  
  * @author  SRA 
  * @brief   This application contains an example which shows how to calculate
  *			 and stream FFT values from onboard analog mic to a PC GUI
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  *
  *
  ******************************************************************************
@endverbatim

@par Application Description 

The UltrasoundFFT shows how to calculate and stream FFT values from the onboard 
analog mic sampled at 192KHz to a PC GUI, available in 'Utilities/UltrasoundFFT'.
Thanks to the very high sampling frequency available, the application can be used
to make condition analysis in the ultrasound frequency domain on any kind of machinery.

'Utilities/UltrasoundFFT' contains 2 folders:
- 'SW_executable'
- 'SW_source'

'SW_Executable' contains a pre-compiled ready-to-use version of the UltrasoundFFT GUI.
User can simply double-click on 'UltrasoundFFT.exe' to launch the application

'SW_source' contains the source code of the UltrasoundFFT GUI.

Both SW_Executable and SW_source are built in QTCreator environment using Qt Creator 4.10.0
- Based on Qt 5.13.1 (MSVC 2017, 32 bit).
DLLs and libraries used are 32-bit based, so if you are using a 64-bit compiler/linker
you must take care of possibile linking compatibility issues.

Once the example starts, the board is in idle state, waiting for a 'start'
command from the PC.
'UltrasoundFFT' PC GUI handle this commands: user can start the PC application and
click on the 'Start' button available on the GUI.

When the command is sent from the PC to the STWIN board through USB, it starts
calculating and streaming the audio FFT.
The Audio Power Spectrum Density is plotted into the GUI and the user can select a 
threshold in energy and frequency to find out the maximum energetic bin in the 
ultrasound frequency range.

Through the GUI it is also possible to store the FFT values into a raw data file,
simply checking the flag 'Save to file'. 
Files are saved in the folder 'Acquisition'; file name is 'data_HHMMSS.dat'

Both folders contains also a Matlab and a Python scripts called 'ReadFFT' to plot the 
spectrogram of the data saved into the 'Acquisition' folder.
'ReadFFT.py' was tested using Python 3.7 on Linux and Windows 10 (Anaconda environment).
'ReadFFT.m' was tested using MATLABv2019a.

