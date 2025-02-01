#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "../common/satellite_data.h"
#include "serial_port.h"
#include "pico_flasher.h"

#include <QMainWindow>
#include <QPlainTextEdit>
#include <QJsonDocument>
#include <QLabel>
#include <QThread>
#include <QString>
#include <nlohmann/json.hpp>
#include <QJsonObject>


using json = nlohmann::json;

class MainWindow : public QMainWindow {
    Q_OBJECT
    QThread timeThread;
    QThread satDataThread;
    QThread uf2FlashThread;
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() = default;
public:
    void setupThreads();
    void setupWindow();
    void setupSensorDataWidget();
private:
    QPlainTextEdit *sensorData;
    QLabel *timeLabel;
    SerialPort *serialPort;
private:
    QLabel *temperature;
    QLabel *pressure;
    QLabel *altitude;
    QLabel *humidity;
    QLabel *accel_x;
    QLabel *accel_y;
    QLabel *accel_z;
    QLabel *gyro_x;
    QLabel *gyro_y;
    QLabel *gyro_z;
    QLabel *mpu_temp;
private:
    QWidget *sensorDataWidget;
public slots:
    void handleSatResults(const QJsonObject &data);
    void handleTimeResults(const QString &time);
    void handleSpError(const QString &error);   
    void handleUf2Flashed(const QProcess::ExitStatus &status);
    void handleUf2Error(const QProcess::ProcessError &error);
signals:
    void startSatThread();
    void startTimeThread();
};

#endif // MAINWINDOW_H
