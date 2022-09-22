/**
******************************************************************************
* @file    GraphicInterface.h
* @author  SRA
* @brief   header file for UltrasoundFFT GraphicInterface
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

#ifndef GRAPHICINTERFACE_H
#define GRAPHICINTERFACE_H

#include <QMainWindow>
#include <QString>
#include <QCheckBox>
#include <QLineEdit>
#include <QTimer>
#include <QDebug>
#include "qcustomplot.h"
#include "HS_DataLog.h"

#define FLOATING_POINT

#ifdef FLOATING_POINT
#define DATASIZE            (4) // datasize in bytes
#else
#define DATASIZE            (2) // datasize in bytes
#endif
#define FFT_LEN             (256)
#define DATALEN             (FFT_LEN * DATASIZE) // DATALEN in bytes
#define PLOTBUFFLEN         (2048 * 10)
#define FFT_BANDWIDTH_HZ    (96000)
#define FFT_BIN_HZ          (FFT_BANDWIDTH_HZ/FFT_LEN)


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    bool USBConnected, logStarted;
    QString * epDescriptors;
    QCheckBox * EP_cB;
    QLineEdit * EP_lE;
    QTimer * isAliveTimer;

    QTimer * plotTimer;
    uint8_t * rawFFTData;
//    uint8_t * plotData;
    QVector<double> plotData;
    uint32_t plotDataCounter;
    uint32_t plotDataInPointer;
    uint32_t plotDataOutPointer;
    QCPItemText *textLabel;
    QCPItemLine *arrow;

    QCPItemLine *fTreshold;
    QCPItemLine *dBTreshold;
    QCPItemRect *backgnd;

    QVector<double> ticks;
    QCPBars *regen;

    QCPColorMap *colorMap;

    int EPn;

    int deviceID = 0;

    QFile logFile;

    int xMaxVisibleValue;

private slots:
    void checkIsAlive(void);

    void toggleConnection(void);
    void toggleStartLog(void);
    void updatePlot(void);
    void resetPlot(void);
    void updateTreshold(void);
    void updateFreqTreshold(void);


};

#endif // GRAPHICINTERFACE_H
