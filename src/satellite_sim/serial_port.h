#ifndef SERIAL_PORT_H
#define SERIAL_PORT_H

#include <QObject>
#include <QSerialPort>


class SerialPort : public QObject 
{
    Q_OBJECT
public:
    explicit SerialPort(QWidget *parent = nullptr);
    ~SerialPort(); 
    void openSerialPort();
    void closeSerialPort();
    bool isOpen();
    void readData();
signals:
    void dataRecieved(const QJsonObject &data);      // Send data to MainWindow
    void errorOccurred(const QString &error);        // Notify MainWindow of error
private slots:
    void onDataRecieved();
    void handleError(QSerialPort::SerialPortError error);
private:
    QSerialPort *sp_serial = nullptr;
    QByteArray dataBuffer;
};

#endif // SERIAL_PORT_H