/**
******************************************************************************
* @file    acquisitionthread.cpp  
* @author  SRA
* @brief   UltrasoundFFT acquisition thread
******************************************************************************
* @attention
*
* Copyright (c) 2022 STMicroelectronics.
* All rights reserved.
*
* This software is licensed under terms that can be found in the LICENSE file in
* the root directory of this software component.
* If no LICENSE file comes with this software, it is provided AS-IS.
*                        
*
******************************************************************************
*/


#include "acquisitionthread.h"
#include <QtWidgets>
#include <iostream>
#include <stdio.h>

#define USB_PACKET_SIZE 4096
acquisitionthread::acquisitionthread(QObject *parent)
    : QThread(parent)
{
abort = false;
}

acquisitionthread::~acquisitionthread()
{
    mutex.lock();
    abort = true;
    mutex.unlock();
    wait();
}

void acquisitionthread::startAcquisitionformEP(QString filename, int EPn, libusb_device_handle * dev_handle)
{
    QMutexLocker locker(&mutex);

    this->filename = filename;
    this->EPn = EPn;
    this->dev_handle = dev_handle;

    data = new unsigned char[USB_PACKET_SIZE];
    dataOutside = new unsigned char[USB_PACKET_SIZE];

    start(TimeCriticalPriority);
}

void acquisitionthread::stopAcquisitionformEP(void)
{
    mutex.lock();
    abort = true;
    mutex.unlock();
}

uint32_t acquisitionthread::readLastAcquiredData(uint8_t * data)
{
    dataMutex.lock();
    uint32_t ret = 0;
    if(dataAvailable == true)
    {
        memcpy(data, dataOutside, USB_PACKET_SIZE);
        dataAvailable = false;
        ret = USB_PACKET_SIZE;
    }
    else
    {
        dataAvailable = false;
        ret = 0;
    }
    dataMutex.unlock();

    return ret;
}

void acquisitionthread::run()
{
    forever {
        mutex.lock();
        mutex.unlock();

        int r, actual,i= 0;
        r = libusb_bulk_transfer(dev_handle, (this->EPn | LIBUSB_ENDPOINT_IN), data, USB_PACKET_SIZE, &actual, 100);
		
        if(r == 0 )
        {
            i++;
        }
else
        {
           libusb_clear_halt	(dev_handle, (this->EPn | LIBUSB_ENDPOINT_IN));
        }

        dataMutex.lock();
        memcpy(dataOutside,data,USB_PACKET_SIZE);
        dataAvailable = true;
        dataMutex.unlock();

        if (abort)
        {
            delete data;
            return;
        }
    }
}
