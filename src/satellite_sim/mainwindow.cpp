#include "mainwindow.h"
#include "../data_preprocessing/json_conversion.h"
#include "q_threads.h"

#include <iostream>
#include <QThread>
#include <QTimer>
#include <QChronoTimer>
#include <QWidget>
#include <QLabel>


MainWindow::MainWindow(QWidget *parent, SatelliteInterface *satelliteInterface) : 
    QMainWindow(parent),
    satelliteInterface(satelliteInterface),
    sensorData(new QPlainTextEdit(this))
{
    setCentralWidget(sensorData);

    timeLabel = new QLabel(this);

    TimeThread *timeWorker = new TimeThread();
    timeWorker->moveToThread(&timeThread);
    connect(&timeThread, &QThread::finished, timeWorker, &QObject::deleteLater);
    connect(timeWorker, &TimeThread::timeUpdated, this, &MainWindow::handleTimeResults);
    connect(this, &MainWindow::startTimeThread, timeWorker, &TimeThread::updateTime);

    timeThread.start();

    emit startTimeThread();

    resize(400, 300);
    setWindowTitle("Satellite Simulator - Initial Test");
}


void MainWindow::handleSatResults(const json &data)
{
    std::cout << "handle results " << std::endl;
}


void MainWindow::handleTimeResults(const QString &time)
{
    timeLabel->setText(time);
}   
