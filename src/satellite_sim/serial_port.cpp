#include "serial_port.h"

#include <QSerialPortInfo>
#include <QSerialPort>
#include <QLabel>
#include <QMessageBox>
#include <QTimer>

#include <chrono>

static constexpr std::chrono::seconds kWriteTimeout = std::chrono::seconds{5};


SerialPort::SerialPort(QWidget *parent)
    : QObject(parent),
        sp_serial(new QSerialPort(this))    
{
   connect(sp_serial, &QSerialPort::readyRead, this, &SerialPort::readData);
   connect(sp_serial, &QSerialPort::errorOccurred, this, &SerialPort::handleError); 
}


SerialPort::~SerialPort()
// Not sure if needed but keep for now
{
    if (sp_serial->isOpen()) { sp_serial->close(); }
}

void SerialPort::openSerialPort() 
{
    if (sp_serial->isOpen()) { sp_serial->close(); }

    sp_serial->setPortName("/dev/tty.usbmodem1101");        // macOS port
    sp_serial->setBaudRate(115200);
    sp_serial->setDataBits(QSerialPort::Data8);
    sp_serial->setParity(QSerialPort::NoParity);
    sp_serial->setStopBits(QSerialPort::OneStop);
    sp_serial->setFlowControl(QSerialPort::NoFlowControl);

    if (!sp_serial->open(QIODevice::ReadWrite)) 
        emit errorOccurred(sp_serial->errorString());
}


void SerialPort::closeSerialPort() 
{
    if (sp_serial->isOpen()) { sp_serial->close(); }
}


void SerialPort::readData()
{
    const QByteArray data = sp_serial->readAll();
    emit dataRecieved(data);
}


void SerialPort::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        emit errorOccurred(sp_serial->errorString());
        closeSerialPort();
    }
}