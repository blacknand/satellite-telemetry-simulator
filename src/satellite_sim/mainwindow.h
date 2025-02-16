#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "../common/satellite_data.h"
#include "serial_port.h"
#include "pico_flasher.h"

// C++ header files
#include <nlohmann/json.hpp>
#include <map>

// Qt specific header files
#include <QMainWindow>
#include <QPlainTextEdit>
#include <QJsonDocument>
#include <QLabel>
#include <QThread>
#include <QString>
#include <QJsonObject>
#include <QPushButton>
#include <QHBoxLayout>
#include <QSignalMapper>
#include <QComboBox>


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
    void setupConnections();
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

    QLabel *errorTerminal;
    QLabel *outputTerminal;

    QPushButton *telemButton;
    
    QWidget *sensorDataWidget;
    QWidget *buttonWidget;
    QWidget *satellite3dWidget;
    QHBoxLayout *consolesLayout;    

    QComboBox *telemetryRate;

    QSignalMapper *signalMapper;

    std::map<std::string, std::string> telemetryRates = {
        {"0.5x", "[COMM] rate_change_0.5x"},
        {"1x", "[COMM] rate_change_1x"},
        {"1.25x", "[COMM] rate_change_1.25x"},
        {"1.5x", "[COMM] rate_change_1.5x"},
        {"2x", "[COMM] rate_change_2x"},
        {"2.5x", "[COMM] rate_change_2.5x"},
        {"3x", "[COMM] rate_change_3x"}
    };

public slots:
    void handleSatResults(const QJsonObject &data);
    void handleTimeResults(const QString &time);
    void handleSpError(const QString &error);   
    void handleUf2Flashed(const QProcess::ExitStatus &status);
    void handleUf2Error(const QProcess::ProcessError &error);
signals:
    void startSatThread();
    void startTimeThread();
    void changeTelemetryRateClicked(const QString &data);
};

#endif // MAINWINDOW_H
