/**
@page UltrasoundFFT application for STWIN

@verbatim
******************************************************************************
* @file    readme.txt  
* @author  SRA - Central Labs
* @version v1.0.1
* @date    03-Apr-2020
* @brief   This application contains an example which shows how to calculate
*			 and stream FFT values from onboard analog mic to a PC GUI
******************************************************************************
* Copyright (c) 2020 STMicroelectronics
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or   
* (at your option) any later version.                                 
*                                                                     
* This program is distributed in the hope that it will be useful,     
* but WITHOUT ANY WARRANTY; without even the implied warranty of      
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the       
* GNU General Public License for more details.                        
*                                                                     
* You should have received a copy of the GNU General Public License   
* along with this program.  If not, see http://www.gnu.org/licenses/. 
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
It is built in QTCreator environment using Qt5.13.1.

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



 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
