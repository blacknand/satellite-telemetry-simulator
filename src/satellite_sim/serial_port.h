#ifndef SERIAL_PORT_H
#define SERIAL_PORT_H

#include <<QObject>
#include <QSerialPort>

class QLabel;
class QTimer;
class MainWindow;

class SerialPort : public QObject 
{
    Q_OBJECT
public:
    explicit SerialPort(QWidget *parent = nullptr);
    ~SerialPort();      // Probably do not need this since serial port will never be closed

private slots:
    void openSerialPort();
    void closeSerialPort();
    void readData();
    void handleError(QSerialPort::SerialPortError error);
private:
    void showStatusMessage(const QString &message);

    SerialPort::SerialPort *sp_ui = nullptr;
    QLabel *sp_status = nullptr;
    QTimer *sp_timer = nullptr;
    QSerialPort *sp_serial = nullptr;
    MainWindow *m_console = nullptr;
};

#endif // SERIAL_PORT_H