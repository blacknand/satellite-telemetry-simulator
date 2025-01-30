#include "mainwindow.h"
#include "../data_preprocessing/json_conversion.h"
#include "q_threads.h"
#include "serial_port.h"
#include "q_output_stream.h"

#include <iostream>
#include <QThread>
#include <QTimer>
#include <QChronoTimer>
#include <QWidget>
#include <QMessageBox>
#include <QLabel>
#include <QTextStream>


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

    FlashPicoUf2File *flashUf2Worker = new FlashPicoUf2File();
    flashUf2Worker->moveToThread(&uf2FlashThread);
    connect(&uf2FlashThread, &QThread::finished, flashUf2Worker, &QObject::deleteLater);
    connect(flashUf2Worker, &FlashPicoUf2File::uf2FileFlashed, this, &MainWindow::handleUf2Flashed);
    connect(flashUf2Worker, &FlashPicoUf2File::errorOccurred, this, &MainWindow::handleUf2Error);

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
    qStdout() << error << Qt::endl;
    qStdout().flush();
}


void MainWindow::handleTimeResults(const QString &time)
{
    timeLabel->setText(time);
}


void MainWindow::handleUf2Flashed(const QProcess::ExitStatus &status)
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Pico UF2 file flashed");

    QString statusText;
    if (status == QProcess::NormalExit) {
        statusText = "The process exited normally.";
    } else {
        statusText = "The process crashed.";
    }


    qStdout() << "[INFO] MainWindow::handleUf2Flashed: " << statusText << Qt::endl;
    qStdout().flush();

    msgBox.setText(statusText);
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();
}


void MainWindow::handleUf2Error(const QProcess::ProcessError &status) 
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Pico UF2 file flash error");

    QString statusText;
    switch (status) {
        case QProcess::FailedToStart:
            statusText = "The process failed to start.";
            break;
        case QProcess::Crashed:
            statusText = "The process crashed.";
            break;
        case QProcess::Timedout:
            statusText = "The process timed out.";
            break;
        case QProcess::WriteError:
            statusText = "An error occurred when attempting to write to the process.";
            break;
        case QProcess::ReadError:
            statusText = "An error occurred when attempting to read from the process.";
            break;
        case QProcess::UnknownError:
        default:
            statusText = "An unknown error occurred.";
            break;
    }

    qStdout() << "[INFO] MainWindow::handleUf2Error: " << statusText << Qt::endl;
    qStdout().flush();

    msgBox.setText(statusText);
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();
}