/**
******************************************************************************
* @file    acquisitionthread.h  
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
  
#ifndef ACQUISITIONTHREAD_H
#define ACQUISITIONTHREAD_H

#include <QWidget>
#include <QMutex>
#include <QSize>
#include <QThread>
#include <QWaitCondition>
#include "libusb.h"
#include <QFile>


class acquisitionthread : public QThread
{
    Q_OBJECT

public:
    acquisitionthread(QObject *parent = 0);
    ~acquisitionthread();
    void startAcquisitionformEP(QString filename, int EPn, libusb_device_handle *dev_handle);
    void stopAcquisitionformEP(void);
    uint32_t readLastAcquiredData(uint8_t * data);


protected:
    void run() override;

private:
    libusb_device_handle *dev_handle;
    QFile * myFile;
    int EPn;
    QString filename;
    QMutex mutex;
    QMutex dataMutex;
    QWaitCondition condition;
    bool restart;
    bool abort;
    unsigned char * data;
    unsigned char * dataOutside;
    bool dataAvailable;
};

#endif // ACQUISITIONTHREAD_H
