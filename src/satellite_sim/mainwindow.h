#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <nlohmann/json.hpp>

#include "../common/satellite_data.h"

#include <QMainWindow>
#include <QPlainTextEdit>
#include <QJsonDocument>


using json = nlohmann::json;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr, 
                        SatelliteInterface *satelliteInterface = nullptr);

public slots:
    void updateData(const json &data);
private:
    QPlainTextEdit *sensorData;
    SatelliteInterface *satelliteInterface;
};

#endif // MAINWINDOW_H
