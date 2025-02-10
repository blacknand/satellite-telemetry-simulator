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
#include <QPushButton>
#include <QHBoxLayout>


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
    void setupConsolesWidget();
    void setupButtonWidget();
    void setup3dWidget();
private:
    QPlainTextEdit *sensorData;
    SerialPort *serialPort;
private:
    QLabel *timeLabel;
    QLabel *temperature;
    QLabel *pressure;
    QLabel *altitude;
    QLabel *humidity;
    QLabel *accelX;
    QLabel *accelY;
    QLabel *accelZ;
    QLabel *gyroX;
    QLabel *gyroY;
    QLabel *gyroZ;
    QLabel *mpuTemperature;
    QLabel *utcLabel;
    
    QPushButton *telemButton;
    
    QWidget *sensorDataWidget;
    QWidget *buttonWidget;
    QWidget *satellite3dWidget;
    QHBoxLayout *consolesLayout;    
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
