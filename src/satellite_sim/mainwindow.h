#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <nlohmann/json.hpp>

#include "../common/satellite_data.h"
#include "serial_port.h"

#include <QMainWindow>
#include <QPlainTextEdit>
#include <QJsonDocument>
#include <QLabel>
#include <QThread>


using json = nlohmann::json;

class MainWindow : public QMainWindow {
    Q_OBJECT
    QThread timeThread;
    QThread satDataThread;
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() = default;
private:
    QPlainTextEdit *sensorData;
    // SatelliteInterface *satelliteInterface;
    QLabel *timeLabel;
    SerialPort *serialPort;
public slots:
    void handleSatResults(const QByteArray &data);
    void handleTimeResults(const QString &time);
    void handleSpError(const QString &error);   
signals:
    void startSatThread();
    void startTimeThread();
};

#endif // MAINWINDOW_H
