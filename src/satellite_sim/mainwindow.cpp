#include "mainwindow.h"
#include "../data_preprocessing/json_conversion.h"
#include "q_threads.h"
#include "serial_port.h"

#include <iostream>
#include <QThread>
#include <QTimer>
#include <QChronoTimer>
#include <QWidget>
#include <QLabel>


MainWindow::MainWindow(QWidget *parent) : 
    QMainWindow(parent),
    sensorData(new QPlainTextEdit(this)),
    serialPort(new SerialPort(this))
{
    resize(1400, 1400); // Set size before placing widgets
    setWindowTitle("Satellite Simulator - Initial Test");

    // Sensor Data Area
    sensorData = new QPlainTextEdit(this);
    sensorData->setStyleSheet("QPlainTextEdit { background-color : black; color : white; }");
    sensorData->setReadOnly(true); // Make read-only if only displaying data
    sensorData->setFixedSize(1200, 1000);
    sensorData->move(200, 200); // Centre it within the window

    // Time Label
    timeLabel = new QLabel("00:00:00", this); // Placeholder time
    timeLabel->setAlignment(Qt::AlignRight | Qt::AlignTop);
    timeLabel->setStyleSheet("QLabel { color : white; font-size: 16px; }");
    timeLabel->setFixedSize(200, 50);
    timeLabel->move(width() - timeLabel->width() - 20, 20);

    // SatDataThread *satWorker = new SatDataThread();
    // satWorker->moveToThread(&satDataThread);
    // connect(&satDataThread, &QThread::finished, satWorker, &QObject::deleteLater);
    // connect(satWorker, &SatDataThread::dataReady, this, &MainWindow::handleSatResults);
    // connect(this, &MainWindow::startSatThread, satWorker, &SatDataThread::processData);
    connect(serialPort, &SerialPort::dataRecived, this, &MainWindow::handleSatResults);
    connect(serialPort, &SerialPort::errorOccurred, this, &MainWindow::handleSpError);

    TimeThread *timeWorker = new TimeThread();
    timeWorker->moveToThread(&timeThread);
    connect(&timeThread, &QThread::finished, timeWorker, &QObject::deleteLater);
    connect(timeWorker, &TimeThread::timeUpdated, this, &MainWindow::handleTimeResults);
    connect(this, &MainWindow::startTimeThread, timeWorker, &TimeThread::updateTime);

    serialPort->openSerialPort();

    timeThread.start();
    // satDataThread.start();

    emit startTimeThread();
    // emit startSatThread();

    resize(1600, 1400);
    setWindowTitle("Satellite Simulator - Initial Test");
}


void MainWindow::handleSatResults(const QByteArray &data)
{
    sensorData->setPlainText(QString::fromUtf8(data));
}


void MainWindow::handleSpError(const QString &error)
{
    std::cout << "fuck" << std::endl;
}


void MainWindow::handleTimeResults(const QString &time)
{
    timeLabel->setText(time);
}