#include "serial_port.h"
#include "q_output_stream.h"

#include <iostream>
#include <QSerialPortInfo>
#include <QSerialPort>
#include <QLabel>
#include <QMessageBox>
#include <QTimer>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QRegularExpression>
#include <QRegularExpressionMatchIterator>
#include <QJsonParseError>
// #include <QByteArray>


SerialPort::SerialPort(QWidget *parent)
    : QObject(parent),
        sp_serial(new QSerialPort(this))    
{
    sp_serial = new QSerialPort(this);
    QByteArray dataBuffer;

    connect(sp_serial, &QSerialPort::readyRead, this, &SerialPort::onDataRecieved);
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
    while (true) {
        // Find the start of a JSON object
        int startIndex = dataBuffer.indexOf("START OF JSON OBJECT");
        if (startIndex == -1) {
            // No start marker found, wait for more data
            break;
        }

        // Find the end of the JSON object
        int endIndex = dataBuffer.indexOf("END OF JSON OBJECT", startIndex);
        if (endIndex == -1) {
            // End marker not found yet, wait for more data
            break;
        }

        // Remove any data before the start marker
        if (startIndex > 0) {
            dataBuffer.remove(0, startIndex);
            startIndex = 0;
            endIndex = dataBuffer.indexOf("END OF JSON OBJECT", startIndex);
        }

        // Extract the JSON object between the markers
        int jsonStart = dataBuffer.indexOf('{', startIndex);
        int jsonEnd = dataBuffer.lastIndexOf('}', endIndex);

        if (jsonStart == -1 || jsonEnd == -1 || jsonEnd < jsonStart) {
            // Invalid JSON structure, remove this block and continue
            dataBuffer.remove(0, endIndex + QString("END OF JSON OBJECT").length());
            continue;
        }

        // Extract JSON text
        QByteArray jsonData = dataBuffer.mid(jsonStart, jsonEnd - jsonStart + 1);

        QJsonParseError parseError;
        QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData, &parseError);
        if (parseError.error != QJsonParseError::NoError) {
            qDebug() << "[ERROR] Failed to parse JSON object: "
                     << parseError.errorString()
                     << " for data: "
                     << jsonData;
        } else {
            QJsonObject jsonObj = jsonDoc.object();
            // Emit the parsed JSON object
            emit dataRecieved(jsonObj);
            
            QByteArray jsonText = QJsonDocument(jsonObj).toJson(QJsonDocument::Compact);
            qDebug() << "[INFO] Parsed JSON object: " << jsonText;
        }

        // Remove the processed data from the buffer
        dataBuffer.remove(0, endIndex + QString("END OF JSON OBJECT").length());
    }
}



void SerialPort::onDataRecieved() 
{
    const QByteArray data = sp_serial->readAll();
    qStdout() << "data recieved" << Qt::endl;
    qStdout().flush();
    dataBuffer.append(data);
    readData();
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