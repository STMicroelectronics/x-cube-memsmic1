# /**
# ******************************************************************************
# * @file    ReadFFT.py
# * @author  SRA
# * @brief   Python routine to read and plot FFT
# ******************************************************************************
# * @attention
# *
# * Copyright (c) 2022 STMicroelectronics.
# * All rights reserved.
# *
# * This software is licensed under terms that can be found in the LICENSE file in
# * the root directory of this software component.
# * If no LICENSE file comes with this software, it is provided AS-IS.
# *                        
# *
# ******************************************************************************
# */

import numpy as np
import matplotlib.pyplot as plt
from glob import glob
        
plt.ion()   
choice = 0     

while choice != 'q':
    fpath = ''
    files = glob('./Acquisitions/*.dat')
    for idx, fpath in enumerate(files):
        print(idx,'-', fpath)
    print('q - Quit')    
    choice = input('Select the file ==> ')
    if choice != 'q':
        fpath = files[int(choice)]
        print('\n',fpath,'\n')
        with open(fpath, 'rb') as fid:
            rawData = np.fromfile(fid,dtype='float32')
            fid.close()
             
        rawData = rawData / 256   
        rawData = 20 * np.log10(rawData)
        Data = np.reshape(rawData,(-1,256))
        lengthData = np.shape(Data)
        lengthrawData = np.shape(rawData)
        timeAxes = np.linspace(0,lengthData[0],lengthData[0])
        fftAxes = np.linspace(0,lengthData[1],lengthData[1])
        fftAxes = fftAxes * 375
        timeAxes = timeAxes / (2 * 375)
		
        Data = Data.transpose()
        
        cf = plt.pcolormesh(timeAxes, fftAxes, Data,cmap = 'jet') 
        plt.colorbar(cf)
        plt.xlabel('sec')
        plt.ylabel('Hz')
        plt.show()
		
