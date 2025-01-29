#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <nlohmann/json.hpp>

#include "../common/satellite_data.h"

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
    SatelliteInterface *satelliteInterface;
    QLabel *timeLabel;
public slots:
    void handleSatResults(const json &data);
    void handleTimeResults(const QString &time);
signals:
    void startSatThread();
    void startTimeThread();
};

#endif // MAINWINDOW_H
