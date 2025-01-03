#include "mainwindow.h"


MainWindow::MainWindow(QWidget *parent) : 
    QMainWindow(parent),
    mpuData(new QPlainTextEdit(this)),
    bmeData(new QPlainTextEdit(this)),
    serialPort(new SerialPort(this))
{
    connect(serialPort, &SerialPort::dataRecieved, this, &MainWindow::updateData);
    connect(serialPort, &SerialPort::errorOccurred, this, &MainWindow::showError);
    setCentralWidget(mpuData);

    resize(400, 300);
    setWindowTitle("Satellite Telemetry Simulator - V0.1");
}


void MainWindow::updateData(const QByteArray &data) {
    mpuData->appendPlainText(QString::fromUtf8(data));
}

void MainWindow::showError(const QString &error) {
    mpuData->appendPlainText("Error: " + error);
}