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
    sp_serial(new QSerialPort(this)),
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
    // sp_serial->setBaudRate(115200);
    // sp_serial->setDataBits(p.dataBits);
    // sp_serial->setParity(p.parity);
    // sp_serial->setStopBits(p.stopBits);
    // sp_serial->setFlowControl(p.flowControl);
}