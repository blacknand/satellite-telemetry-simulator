#include "mainwindow.h"
#include "../data_preprocessing/json_conversion.h"
#include "q_threads.h"
#include "q_satellite_factory.h"

#include <iostream>
#include <QThread>
#include <QTimer>
#include <QChronoTimer>
#include <QWidget>
#include <QLabel>


MainWindow::MainWindow(QWidget *parent) : 
    QMainWindow(parent),
    sensorData(new QPlainTextEdit(this))
{
    setCentralWidget(sensorData);

    timeLabel = new QLabel(this);
    sensorData = new QPlainTextEdit(this);

    SatDataThread *satWorker = new SatDataThread(make_satellite_sensors());
    satWorker->moveToThread(&satDataThread);
    connect(&satDataThread, &QThread::finished, satWorker, &QObject::deleteLater);
    connect(satWorker, &SatDataThread::dataReady, this, &MainWindow::handleSatResults);
    connect(this, &MainWindow::startSatThread, satWorker, &SatDataThread::processData);

    TimeThread *timeWorker = new TimeThread();
    timeWorker->moveToThread(&timeThread);
    connect(&timeThread, &QThread::finished, timeWorker, &QObject::deleteLater);
    connect(timeWorker, &TimeThread::timeUpdated, this, &MainWindow::handleTimeResults);
    connect(this, &MainWindow::startTimeThread, timeWorker, &TimeThread::updateTime);

    timeThread.start();
    satDataThread.start();

    emit startTimeThread();
    emit startSatThread();

    resize(400, 300);
    setWindowTitle("Satellite Simulator - Initial Test");
}


void MainWindow::handleSatResults(const json &data)
{
    sensorData->setPlainText(QString::fromStdString(data.dump(4)));
}


void MainWindow::handleTimeResults(const QString &time)
{
    timeLabel->setText(time);
}   
