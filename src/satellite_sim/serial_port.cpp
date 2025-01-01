#include "serial_port.h"

#include <QSerialPortInfo>
#include <QSerialPort>
#include <QLabel>
#include <QMessageBox>
#include <QTimer>

#include <chrono>

static constexpr std::chrono::seconds kWriteTimeout = std::chrono::seconds{5};


SerialPort::SerialPort(QWidget *parent) : 
    QMainWindow(parent),
    sp_ui(new SerialPort::SerialPort),
    sp_status(new QLabel),
    sp_timer(new QTimer),
    sp_serial(new QSerialPort(this))
{
    connect(sp_serial, &QSerialPort::errorOccured, this, &SerialPort::handleError);
    connect(sp_serial, &QTimer::timeout, this, &SerialPort::handleWriteTimeout);
    sp_timer->setSingleShot(true);
    connect(sp_serial, &QSerialPort::readyRead, this, &SerialPort::readData);
    connect(m_console, &Console::getData, this, &SerialPort::writeData);
}


SerialPort::~SerialPort()
// Not sure if needed but keep for now
{
    delete sp_ui;
}


void SerialPort::OpenSerialPort() 
{
    if (sp_serial->isOpen()) { closeSerialPort(); }

    sp_serial->setPortName("/dev/tty.usbmodem1101");
    sp_serial->setBaudRate(115200);
    sp_serial->setDataBits(QSerialPort::Data8);
    sp_serial->setParity(QSerialPort::NoParity);
    sp_serial->setStopBits(QSerialPort::OneStop);
    sp_serial->setFlowControl(QSerialPort::NoFlowControl);

    if (sp_serial->open(QIODevice::ReadWrite)) {
        showStatusMessage(tr("Connected to %1 : %2, %3, %4, %5, %6")
                            .arg(p.name, p.stringBaudRate, p.stringDataBits,
                                p.stringParity, p.stringStopBits, p.stringFlowControl));
    } else {
        QMessageBox::critical(this, tr("Error"), sp_serial->errorString());
        showStatusMessage(tr("Open error"));
    }
}


void SerialPort::CloseSerialPort() 
{
    if (sp_serial->isOpen()) {
        sp_serial->close();
        showStatusMessage(tr("Disconnected"));
    }
}


void MainWindow::readData()
{
    const QByteArray data = sp_serial->readAll();
    // TODO: instead of m_console it is going to have to go into mainwindow
    m_console->putData(data);
}


void MainWindow::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        QMessageBox::critical(this, tr("Critical Error"), sp_serial->errorString());
        closeSerialPort();
    }
}


void MainWindow::showStatusMessage(const QString &message)
{
    sp_status->setText(message);
}