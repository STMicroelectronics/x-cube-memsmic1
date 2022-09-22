% /**
% ******************************************************************************
% * @file    ReadFFT.m
% * @author  SRA
% * @brief   MATLAB routine to read and plot FFT
% ******************************************************************************
% * @attention
% *
% * Copyright (c) 2022 STMicroelectronics.
% * All rights reserved.
% *
% * This software is licensed under terms that can be found in the LICENSE file in
% * the root directory of this software component.
% * If no LICENSE file comes with this software, it is provided AS-IS.
% *                        
% *
% ******************************************************************************
% */


fclose all;
clear all;
close all;
clc;

[fJ,fpath] = uigetfile('.\Acquisitions\*.dat');
if iscell(fJ)
    fJs = strings(1, length(fJ));
    for ii = 1:length(fJ)
        fJs(ii) = cell2mat(fJ(ii));
    end
else
    fJs = strings(1, 1);
    fJs(1) = fJ;
end

for ii = 1:length(fJs)
    audioFile = fJs(ii);
    audioFile = strcat(fpath, audioFile);
    
    fid = fopen(audioFile);
    rawData = fread(fid,inf, 'single');
    fclose(fid);
    
    rawData = rawData / 256;
    rawData = 20 * log10(rawData);
    Data = reshape(rawData,256,[]);
    lengthData = size(Data);
    lengthrawData = size(rawData);
    timeAxes = linspace(0,lengthData(2),lengthData(2));
    fftAxes = linspace(0,lengthData(1),lengthData(1));
    fftAxes = fftAxes * 375;
    timeAxes = timeAxes / (2 * 375);
    
    mesh(timeAxes, fftAxes, Data);
    view(2)
    colorbar
    xlabel('sec')
    ylabel('Hz')
    
end
