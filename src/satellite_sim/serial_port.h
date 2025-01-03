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

signals:
    void dataRecived(const QByteArray &data);      // Send data to MainWindow
    void errorOccurred(const QString &error);        // Notify MainWindow of error
private slots:
    void readData();
    void handleError(QSerialPort::SerialPortError error);
private:
    QSerialPort *sp_serial = nullptr;
};

#endif // SERIAL_PORT_H