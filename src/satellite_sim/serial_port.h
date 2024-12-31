#ifndef SERIAL_PORT_H
#define SERIAL_PORT_H

class SerialPort {
public:
    SerialPort(QWidget *parent = nullptr);
    OpenSerialPort();
    CloseSerialPort();
    ReadData();
private:

};

#endif // SERIAL_PORT_H