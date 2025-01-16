#include "mainwindow.h"
#include "../data_preprocessing/json_conversion.h"

#include <iostream>


MainWindow::MainWindow(QWidget *parent, SatelliteInterface *satelliteInterface) : 
    QMainWindow(parent),
    satelliteInterface(satelliteInterface),
    sensorData(new QPlainTextEdit(this))
{
    setCentralWidget(sensorData);

    SatelliteData data = satelliteInterface->get_satellite_data();
    json j = data;
    updateData(j);

    resize(400, 300);
    setWindowTitle("Satellite Simulator - Initial Test");
}


void MainWindow::updateData(const json &data) {
    QString sensorDataStr = QString::fromStdString(data.dump(4));
    sensorData->setPlainText(sensorDataStr);
}