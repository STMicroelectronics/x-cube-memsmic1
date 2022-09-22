/**
******************************************************************************
* @file    GraphicInterface.cpp
* @author  SRA
* @brief   UltrasoundFFT GraphicInterface
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

#include "GraphicInterface.h"
#include "ui_GraphicInterface.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    this->USBConnected = false;
    this->logStarted = false;

    plotTimer = new QTimer(this);
    isAliveTimer = new QTimer;
    this->setWindowIcon(QIcon(":STMicroelectronics.ico"));

    ui->setupUi(this);

    this->ui->freqtresholdslider->valueChanged(20000);
    this->ui->TresholdSlider->valueChanged(-120);
    this->ui->label_3->setVisible(false);
    this->ui->Max_Label->setVisible(false);


    ui->pB_StartLog->setEnabled(false);
    ui->label->hide();
    ui->label_2->hide();
    ui->pB_StartLog->hide();


    plotDataCounter = 0;
    plotDataOutPointer = 0;
    plotDataInPointer = 0;

    connect(plotTimer, SIGNAL(timeout()), this, SLOT(updatePlot()));
    connect(ui->pB_Connect, SIGNAL(clicked()), SLOT(toggleConnection()));
    connect(ui->pB_StartLog, SIGNAL(clicked()), SLOT(toggleStartLog()));
    connect(this->isAliveTimer, SIGNAL(timeout()), SLOT(checkIsAlive()));

    connect(ui->TresholdSlider, SIGNAL(valueChanged(int)), SLOT(updateTreshold()));
    connect(ui->freqtresholdslider, SIGNAL(valueChanged(int)), SLOT(updateFreqTreshold()));

    // set dark background gradient:
    QLinearGradient gradient(0, 0, 0, 400);
    gradient.setColorAt(0, QColor(90, 90, 90));
    gradient.setColorAt(0.38, QColor(105, 105, 105));
    gradient.setColorAt(1, QColor(70, 70, 70));
    ui->QCPWidget->setBackground(QBrush(gradient));

    // create empty bar chart objects:
    this->regen = new QCPBars(ui->QCPWidget->xAxis, ui->QCPWidget->yAxis);
    regen->setAntialiased(true); // gives more crisp, pixel aligned bar borders
    regen->setStackingGap(1);
    regen->setName("Regenerative");
    regen->setPen(QPen(QColor(0, 168, 140).lighter(130)));
    regen->setBrush(QColor(0, 168, 140));
    regen->setBaseValue(-150);

    QVector<QString> xAxisLabels;
    int xTicksCounter = FFT_LEN;
    int xMaxValue = FFT_BANDWIDTH_HZ;
    this->xMaxVisibleValue = 80000;

    ui->freqtresholdslider->setMaximum(this->xMaxVisibleValue);

    int fftBinSize = (xMaxValue/xTicksCounter);

    for (int i = 0; i < xTicksCounter; i++)
    {
        this->ticks << i;
        if (i%10 == 0)
        {
            xAxisLabels << QString::number((xMaxValue/xTicksCounter)*i);
        }
        else
        {
            xAxisLabels << "";
        }
    }

    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
    textTicker->addTicks(ticks, xAxisLabels);
    textTicker->setTickStepStrategy(QCPAxisTicker::tssMeetTickCount);
    textTicker->setTickCount(10);

    ui->QCPWidget->xAxis->setTicker(textTicker);
    ui->QCPWidget->xAxis->setTickLabelRotation(60);
    ui->QCPWidget->xAxis->setSubTicks(false);
    ui->QCPWidget->xAxis->setTickLength(0, 2);
    ui->QCPWidget->xAxis->setRange(1, xMaxVisibleValue/fftBinSize);
    ui->QCPWidget->xAxis->setLabel("Frequency [Hz]");
    ui->QCPWidget->xAxis->setBasePen(QPen(Qt::white));
    ui->QCPWidget->xAxis->setTickPen(QPen(Qt::white));
    ui->QCPWidget->xAxis->grid()->setVisible(false);
    ui->QCPWidget->xAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));
    ui->QCPWidget->xAxis->setTickLabelColor(Qt::white);
    ui->QCPWidget->xAxis->setLabelColor(Qt::white);

    // prepare y axis:
    ui->QCPWidget->yAxis->setRange(-150, -40);
    ui->QCPWidget->yAxis->setPadding(5); // a bit more space to the left border
    ui->QCPWidget->yAxis->setLabel("Power Spectral Density [dB/Hz]");
    ui->QCPWidget->yAxis->setBasePen(QPen(Qt::white));
    ui->QCPWidget->yAxis->setTickPen(QPen(Qt::white));
    ui->QCPWidget->yAxis->setSubTickPen(QPen(Qt::white));
    ui->QCPWidget->yAxis->grid()->setSubGridVisible(true);
    ui->QCPWidget->yAxis->setTickLabelColor(Qt::white);
    ui->QCPWidget->yAxis->setLabelColor(Qt::white);
    ui->QCPWidget->yAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::SolidLine));
    ui->QCPWidget->yAxis->grid()->setSubGridPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));

    // Add data:
    QVector<double> fossilData, nuclearData, regenData;
    regenData   << 0.06*10.5 << 0.05*5.5 << 0.04*5.5 << 0.06*5.8 << 0.02*5.2 << 0.07*4.2 << 0.25*11.2 <<22 <<24;

    // add the text label at the top:
    textLabel = new QCPItemText(ui->QCPWidget);
    textLabel->setPositionAlignment(Qt::AlignTop|Qt::AlignHCenter);
    textLabel->position->setType(QCPItemPosition::ptAxisRectRatio);
    textLabel->position->setCoords(0.5, 0); // place position at center/top of axis rect
    textLabel->setText("-----");
    textLabel->setPadding(QMargins(0,0,0,5));
    textLabel->setColor(QColor(Qt::white));
    textLabel->setFont(QFont(font().family(), 13)); // make font a bit larger
    textLabel->setVisible(false);

    // add the arrow:
    arrow = new QCPItemLine(ui->QCPWidget);
    arrow->start->setParentAnchor(textLabel->bottom);
    arrow->end->setCoords(128, 80); // point to (4, 1.6) in x-y-plot coordinates
    arrow->setPen(QPen(Qt::white));
    arrow->setHead(QCPLineEnding::esNone);
    arrow->setVisible(false);

    /* Frequency threshold */
    QPen fThresholdPen;

    fThresholdPen.setStyle(Qt::DashDotLine);
    fThresholdPen.setWidth(1);
    fThresholdPen.setBrush(Qt::white);
    fThresholdPen.setCapStyle(Qt::RoundCap);
    fThresholdPen.setJoinStyle(Qt::RoundJoin);
    fTreshold = new QCPItemLine(ui->QCPWidget);
    fTreshold->start->setCoords(this->ui->freqtresholdslider->value()/FFT_BIN_HZ, -150);
    fTreshold->end->setCoords(this->ui->freqtresholdslider->value()/FFT_BIN_HZ, 0); // point to (4, 1.6) in x-y-plot coordinates
    fTreshold->setPen(fThresholdPen);

    /* Module threshold */
    QPen dBThresholdPen;

    dBThresholdPen.setStyle(Qt::DashDotLine);
    dBThresholdPen.setWidth(1);
    dBThresholdPen.setBrush(Qt::white);
    dBThresholdPen.setCapStyle(Qt::RoundCap);
    dBThresholdPen.setJoinStyle(Qt::RoundJoin);
    dBTreshold = new QCPItemLine(ui->QCPWidget);
    dBTreshold->start->setCoords(0, this->ui->TresholdSlider->value());
    dBTreshold->end->setCoords(256, this->ui->TresholdSlider->value()); // point to (4, 1.6) in x-y-plot coordinates
    dBTreshold->setPen(dBThresholdPen);

    /* plot background in area over thresholds */
    ui->QCPWidget->addLayer("Backgnd",ui->QCPWidget->layer("grid"), QCustomPlot::limBelow);
    ui->QCPWidget->setCurrentLayer("Backgnd");
    backgnd = new QCPItemRect(ui->QCPWidget);
    backgnd->topLeft->setCoords(ui->freqtresholdslider->value()/FFT_BIN_HZ, 0);
    backgnd->bottomRight->setCoords(256,ui->TresholdSlider->value());
    backgnd->setBrush(QBrush(Qt::darkGray, Qt::Dense4Pattern));
    backgnd->setPen(Qt::NoPen);

    ui->QCPWidget->setOpenGl(true);
    ui->QCP_Spectrogram->hide();

    plotData.resize(FFT_LEN);
    for(int i=0;i<plotData.size();i++)
    {
        plotData[i]=0;
    }

}

MainWindow::~MainWindow()
{
    if(this->USBConnected)
    {
        this->toggleConnection();
    }
    delete plotTimer;
    delete ui;
}

void MainWindow::updatePlot(void)
{
    static uint64_t timeout = 0;
    timeout ++;
    int size = 0, actual=0;

    hs_datalog_get_available_data_size(0, 0, &size);

    qDebug()<< "SIZE: " <<size;

    if(size > 0)
    {
        timeout = 0;
        rawFFTData = new unsigned char[static_cast<unsigned int>(size)];

        hs_datalog_get_data( 0, 0, rawFFTData, size, &actual);

        static volatile uint16_t delay = 200;

        double max_value = 0;
        int max_value_idx = -1;

        int i,j;
        double d_data;
        int data_size = size/DATASIZE;
#if (1)
        for (i=0; i<data_size; i++)
        {
            d_data = static_cast<double>((reinterpret_cast<float *>(rawFFTData))[i]);
            j = i%(FFT_LEN);
            d_data = d_data * d_data;
            d_data = d_data / (FFT_LEN * 2 * FFT_BANDWIDTH_HZ);
            if(ui->cB_LowPassFilter->isChecked())
            {
                plotData[j] = plotData[j] * 0.97 + d_data * 0.03;
            }
            else
            {
                plotData[j] = d_data;
            }

        }
#else
        for (i=0; i<data_size; i++)
        {
            d_data = static_cast<double>((reinterpret_cast<float *>(rawFFTData))[i]);
            j = i%(FFT_LEN);
            plotData[j] = d_data;
        }
#endif
        QVector<double> plotData_dB(FFT_LEN);
        int visibleFreqBin = this->xMaxVisibleValue/FFT_BIN_HZ;
        int freqThresholdBin = this->ui->freqtresholdslider->value()/FFT_BIN_HZ;
        int moduleThreshold = ui->TresholdSlider->value();

        for (i = 0; i<FFT_LEN; i++)
        {
#ifdef FLOATING_POINT
            plotData_dB[i] = 150 + 10*log10(plotData[i]);
#else
            float current_abs = 150 + 10*log10((float)(((int16_t *)plotData)[plotDataOutPointer])/32768);
#endif
            if(i<visibleFreqBin && i>freqThresholdBin && plotData_dB.at(i)>(moduleThreshold+150) && plotData_dB.at(i)>max_value)
            {
                max_value = plotData_dB.at(i);
                max_value_idx = i;
                arrow->end->setCoords(max_value_idx, -150 + max_value + 1);
            }
        }
        if(max_value_idx != -1 && ui->cB_PeakDetetcor->isChecked())
        {
            arrow->setVisible(true);
            textLabel->setVisible(true);
            textLabel->setText("Detected peak: " + QString::number((max_value_idx)*FFT_BIN_HZ) + "Hz");
            ui->Max_Label->setText(QString::number(max_value_idx*FFT_BIN_HZ));
//            delay = 200;
        }
        else
        {
            if(delay-- == 0)
            {
                ui->Max_Label->setText("-----");
                arrow->setVisible(false);
                textLabel->setVisible(false);
                delay = 200;
            }
        }

        regen->setData(ticks, plotData_dB);

        if(ui->cB_SaveToFile->isChecked())
        {
            logFile.write(reinterpret_cast<char*>(rawFFTData), size);
        }

        ui->QCPWidget->replot();
        QCoreApplication::processEvents();
        delete rawFFTData;
    }
    if(timeout >750)
    {
        this->toggleConnection();
    }
}

void MainWindow::resetPlot(void)
{
    QVector<double> data;

    for (int i = 0; i<DATALEN/8; i++ )
    {
        data << 0;

        plotDataOutPointer = (plotDataOutPointer + 2) % (PLOTBUFFLEN/4);
    }

    regen->setData(ticks, data);

    ui->QCPWidget->replot();
    QCoreApplication::processEvents();
}

void MainWindow::toggleConnection(void)
{
    if(this->USBConnected)
    {
        toggleStartLog();
        ui->pB_Connect->setText("START");
        ui->pB_StartLog->setEnabled(false);

        hs_datalog_stop_log(0);
        hs_datalog_close();

        this->USBConnected = false;
        if(ui->cB_SaveToFile->isChecked())
        {
            logFile.close();
        }
        ui->cB_SaveToFile->setEnabled(true);
    }
    else
    {
        char * version;
        hs_datalog_get_version(&version);

        if(hs_datalog_open() != ST_HS_DATALOG_OK)
        {
            qDebug() << "Error occurred while initializing datalog\n";
        }

        int nDevices = 0;
        if(hs_datalog_get_device_number(&nDevices) != ST_HS_DATALOG_OK)
        {
            qDebug() << "Error occurred while retrieving device number\n";
        }

        if(nDevices == 0)
        {
            qDebug() << "No devices, exiting\n";
            return;
        }

        /* If multiple devices are present, address only device with id = 0 */
//        int deviceID = 0;
        int nSensors = 0;

        char *tempDev;

        if(hs_datalog_get_device_descriptor(deviceID, &tempDev) != ST_HS_DATALOG_OK)
        {
            qDebug() << "Error occurred while retrieving device descriptor\n";
        }

        /* Free memory */
        if(hs_datalog_free(tempDev) != ST_HS_DATALOG_OK)
        {
        }

        if(hs_datalog_get_sensor_number(deviceID, &nSensors) != ST_HS_DATALOG_OK)
        {
        }

        if(ui->cB_SaveToFile->isChecked())
        {
            QDateTime currentTime(QDate::currentDate(),QTime::currentTime());
            logFile.setFileName("./Acquisitions/" + currentTime.toString(QStringView(QString("yyyyMMdd_hhmmss"))) + ".dat");
            logFile.open(QFile::WriteOnly);
        }

        ui->pB_Connect->setText("STOP");
        ui->pB_StartLog->setEnabled(true);
        this->USBConnected = true;
        ui->cB_SaveToFile->setEnabled(false);
        toggleStartLog();
    }
}

void MainWindow::checkIsAlive(void)
{

}

void MainWindow::toggleStartLog(void)
{
    if(this->logStarted)
    {
        arrow->setVisible(false);
        textLabel->setVisible(false);

        /* Stop logging */
        hs_datalog_stop_log(deviceID);

        ui->statusBar->showMessage("Log stopped");
        ui->pB_StartLog->setText("Start Log");
        this->logStarted = 0;
        this->plotTimer->stop();
        this->resetPlot();
    }
    else
    {
        plotDataCounter=0;
        plotDataInPointer=0;
        plotDataOutPointer=0;

        if(hs_datalog_set_sensor_active(deviceID, 0, true) != ST_HS_DATALOG_OK)
        {

        }

        if(hs_datalog_set_sub_sensor_active(deviceID, 0, 0, true) != ST_HS_DATALOG_OK)
        {

        }

        hs_datalog_start_log(deviceID);

        QThread::msleep(100);
        ui->statusBar->showMessage("Log started");
        ui->pB_StartLog->setText("Stop Log");
        this->logStarted = 1;

        plotTimer->start(3);
    }
}

void MainWindow::updateFreqTreshold(void)
{
    fTreshold->start->setCoords(this->ui->freqtresholdslider->value()/FFT_BIN_HZ, -150);
    fTreshold->end->setCoords(this->ui->freqtresholdslider->value()/FFT_BIN_HZ, 0); // point to (4, 1.6) in x-y-plot coordinates
    backgnd->topLeft->setCoords(ui->freqtresholdslider->value()/FFT_BIN_HZ, 0);
    backgnd->bottomRight->setCoords(256,ui->TresholdSlider->value());
    ui->QCPWidget->replot();
}

void MainWindow::updateTreshold(void)
{
    dBTreshold->start->setCoords(0, this->ui->TresholdSlider->value());
    dBTreshold->end->setCoords(256, this->ui->TresholdSlider->value()); // point to (4, 1.6) in x-y-plot coordinates
    backgnd->topLeft->setCoords(ui->freqtresholdslider->value()/FFT_BIN_HZ, 0);
    backgnd->bottomRight->setCoords(256,ui->TresholdSlider->value());
    ui->QCPWidget->replot();
}
