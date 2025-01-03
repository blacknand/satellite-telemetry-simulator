#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "serial_port.h"

#include <QMainWindow>
#include <QPlainTextEdit>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

public slots:
    void updateData(const QByteArray &data);
    void showError(const QString &error);
private:
    QPlainTextEdit *mpuData;
    QPlainTextEdit *bmeData;
    SerialPort *serialPort;
};

#endif // MAINWINDOW_H
