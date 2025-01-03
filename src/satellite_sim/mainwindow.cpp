#include "mainwindow.h"

#include <iostream>


MainWindow::MainWindow(QWidget *parent) : 
    QMainWindow(parent),
    mpuData(new QPlainTextEdit(this)),
    bmeData(new QPlainTextEdit(this)),
    serialPort(new SerialPort(this))
{
    connect(serialPort, &SerialPort::dataRecived, this, &MainWindow::updateData);
    connect(serialPort, &SerialPort::errorOccurred, this, &MainWindow::showError);
    setCentralWidget(mpuData);

    serialPort->openSerialPort();
    std::cout << "Serial port opened? " << serialPort->isOpen() << std::endl;

    resize(400, 300);
    setWindowTitle("Satellite Telemetry Simulator - Serial port test");
}


void MainWindow::updateData(const QByteArray &data) {
    mpuData->appendPlainText(QString::fromUtf8(data));
}

void MainWindow::showError(const QString &error) {
    mpuData->appendPlainText("Error: " + error);
}