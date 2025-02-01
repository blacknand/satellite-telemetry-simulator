#include "serial_port.h"
#include "q_output_stream.h"

#include <QSerialPortInfo>
#include <QSerialPort>
#include <QLabel>
#include <QMessageBox>
#include <QTimer>
#include <iostream>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>


SerialPort::SerialPort(QWidget *parent)
    : QObject(parent),
        sp_serial(new QSerialPort(this))    
{
    sp_serial = new QSerialPort(this);

   connect(sp_serial, &QSerialPort::readyRead, this, &SerialPort::readData);
   connect(sp_serial, &QSerialPort::errorOccurred, this, &SerialPort::handleError); 

    // QTimer *readTimer = new QTimer(this);
    // connect(readTimer, &QTimer::timeout, this, &SerialPort::readData);
    // readTimer->start(1000); // Read every 50ms instead of every available byte
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
    emit dataReceived(data);


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