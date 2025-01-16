// Serial port is not used, is kept here just if it is needed in any case

#include "serial_port.h"

#include <QSerialPortInfo>
#include <QSerialPort>
#include <QLabel>
#include <QMessageBox>
#include <QTimer>
#include <iostream>


SerialPort::SerialPort(QWidget *parent)
    : QObject(parent),
        sp_serial(new QSerialPort(this))    
{
   connect(sp_serial, &QSerialPort::readyRead, this, &SerialPort::readData);
   connect(sp_serial, &QSerialPort::errorOccurred, this, &SerialPort::handleError); 
}


SerialPort::~SerialPort()
{
    if (sp_serial->isOpen()) { sp_serial->close(); }
}


void SerialPort::openSerialPort() 
{
    if (sp_serial->isOpen()) { sp_serial->close(); }

    sp_serial->setPortName("/dev/tty.usbmodem1101");        // macOS port, hardcoded for meantime
    sp_serial->setBaudRate(115200);
    sp_serial->setDataBits(QSerialPort::Data8);
    sp_serial->setParity(QSerialPort::NoParity);
    sp_serial->setStopBits(QSerialPort::OneStop);
    sp_serial->setFlowControl(QSerialPort::NoFlowControl);

    std::cout << "serial port /dev/tty.usbmodem1101 opened" << std::endl;

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
    // std::cout << "data received: " << data.toStdString() << std::endl;
    emit dataRecived(data);
}


void SerialPort::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        std::cout << "error occured: " << sp_serial->errorString().toStdString() << std::endl;
        emit errorOccurred(sp_serial->errorString());
        closeSerialPort();
    }
}


bool SerialPort::isOpen() 
{
    return sp_serial->isOpen();
}