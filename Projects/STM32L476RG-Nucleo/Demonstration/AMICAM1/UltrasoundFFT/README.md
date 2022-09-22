##__UltrasoundFFT application__

The UltrasoundFFT shows how to calculate and stream FFT values from the onboard 
analog mic sampled at 192KHz to a PC GUI, available in 'Utilities/UltrasoundFFT'.
Thanks to the very high sampling frequency available, the application can be used
to make condition analysis in the ultrasound frequency domain on any kind of machinery.

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

The folder 'Utilities/UltrasoundFFT' contains also a Matlab and a Python scripts called
'ReadFFT' to plot the spectrogram of the data saved into the 'Acquisition' folder.
'ReadFFT.py' was tested using Python 3.7 on Linux and Windows 10 (Anaconda environment).
'ReadFFT.m' was tested using MATLABv2019a.


### __Hardware and Software environment__

  - This application runs on X-NUCLEO-AMICAM1 on top of a NUCLEO-L476RG.
  - STM32 Virtual COM Port Driver for Windows can be downloaded from st.com - STSW-STM32102

    
### __How to use it?__

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example
 - In order to stream audio via USB, connect X-NUCLEO-AMICAM1 USB connector to a host PC

