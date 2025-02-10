#include "mainwindow.h"
#include "../data_preprocessing/json_conversion.h"
#include "q_threads.h"
#include "serial_port.h"
#include "q_output_stream.h"

#include <iostream>
#include <QThread>
#include <QTimer>
#include <QChronoTimer>
#include <QWidget>
#include <QMessageBox>
#include <QLabel>
#include <QTextStream>
#include <QGridLayout>
#include <QJsonObject>
#include <QJsonDocument>
#include <QPushButton>


MainWindow::MainWindow(QWidget *parent) : 
    QMainWindow(parent),
    serialPort(new SerialPort(this))
{
    resize(1800, 1400);
    setWindowTitle("Satellite Simulator - Initial Test");

    // Create QLabel objects (only once)
    temperature = new QLabel("Temperature: 0", this);
    pressure = new QLabel("Pressure: 0", this);
    altitude = new QLabel("Altitude: 0", this);
    humidity = new QLabel("Humidity: 0", this);
    accelX = new QLabel("Accel X: 0", this);
    accelY = new QLabel("Accel Y: 0", this);
    accelZ = new QLabel("Accel Z: 0", this);
    gyroX = new QLabel("Gyro X: 0", this);
    gyroY = new QLabel("Gyro Y: 0", this);
    gyroZ = new QLabel("Gyro Z: 0", this);
    mpuTemperature = new QLabel("MPU Temp: 0", this);

    utcLabel = new QLabel("00:00:00", this);
    timeLabel = new QLabel("00:00:00", this);

    // telemButton = new QPushButton("&TEST", this);

    setupWindow();
    setupThreads();
}


void MainWindow::setupWindow()
{
    QWidget *mainWidget = new QWidget(this);
    QGridLayout *mainLayout = new QGridLayout(mainWidget);

    setupSensorDataWidget();  // Create and set up sensor data widget
    setupConsolesWidget();    // Create and set up consoles widget
    setupButtonWidget();      // Create and set up button widget
    setup3dWidget();          // Create and set up 3D widget

    // Adjusting widget sizes
    sensorDataWidget->setFixedWidth(350);   // Reduce telemetry data width
    buttonWidget->setFixedWidth(400);       // Reduce buttons widget width
    errorTerminal->setFixedWidth(400);      // Narrow error terminal
    outputTerminal->setFixedWidth(400);     // Narrow output terminal

    // Add widgets to the grid layout with fixed positioning
    mainLayout->addWidget(sensorDataWidget, 0, 0, 2, 1);  // Telemetry data on the left
    mainLayout->addWidget(satellite3dWidget, 0, 1);       // 3D Satellite in the center
    mainLayout->addWidget(buttonWidget, 0, 2, 1, 1);      // Buttons on the right
    mainLayout->addWidget(errorTerminal, 1, 1, 1, 1);     // Error terminal (left in bottom row)
    mainLayout->addWidget(outputTerminal, 1, 2, 1, 1);    // Output terminal (right in bottom row)

    mainWidget->setLayout(mainLayout);
    setCentralWidget(mainWidget);
}

void MainWindow::setupSensorDataWidget()
{
    sensorDataWidget = new QWidget(this);
    QVBoxLayout *sensorDataLayout = new QVBoxLayout(sensorDataWidget);

    sensorDataLayout->addWidget(temperature);
    sensorDataLayout->addWidget(pressure);
    sensorDataLayout->addWidget(altitude);
    sensorDataLayout->addWidget(humidity);
    sensorDataLayout->addWidget(accelX);
    sensorDataLayout->addWidget(accelY);
    sensorDataLayout->addWidget(accelZ);
    sensorDataLayout->addWidget(gyroX);
    sensorDataLayout->addWidget(gyroY);
    sensorDataLayout->addWidget(gyroZ);
    sensorDataLayout->addWidget(mpuTemperature);
    sensorDataLayout->addWidget(utcLabel);

    // Add border for clarity
    sensorDataWidget->setStyleSheet("border: 2px solid red;");
}

void MainWindow::setupConsolesWidget()
{
    // Create console widgets
    errorTerminal = new QLabel("Error Terminal Placeholder", this);
    outputTerminal = new QLabel("Output Terminal Placeholder", this);

    // Ensure they have fixed sizes to prevent overlap
    errorTerminal->setFixedSize(180, 100);
    outputTerminal->setFixedSize(180, 100);

    // Add borders for clarity
    errorTerminal->setStyleSheet("border: 2px solid blue;");
    outputTerminal->setStyleSheet("border: 2px solid blue;");
}

void MainWindow::setupButtonWidget()
{
    buttonWidget = new QWidget(this);
    QVBoxLayout *buttonLayout = new QVBoxLayout(buttonWidget);

    QPushButton *exampleButton1 = new QPushButton("Change telemetry collection rate", this);
    QPushButton *exampleButton2 = new QPushButton("Example Button", this);
    buttonLayout->addWidget(exampleButton1);
    buttonLayout->addWidget(exampleButton2);

    // Reduce width of buttons and button widget
    buttonWidget->setFixedWidth(200);
    exampleButton1->setFixedWidth(180);
    exampleButton2->setFixedWidth(180);

    // Add border for clarity
    buttonWidget->setStyleSheet("border: 2px solid green;");
}

void MainWindow::setup3dWidget()
{
    satellite3dWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(satellite3dWidget);

    QLabel *satellitePlaceholder = new QLabel("3D Satellite Placeholder", this);
    satellitePlaceholder->setAlignment(Qt::AlignCenter);
    layout->addWidget(satellitePlaceholder);

    // Add border for clarity
    satellite3dWidget->setStyleSheet("border: 2px solid yellow;");
}



void MainWindow::setupThreads() 
{
    connect(serialPort, &SerialPort::dataRecieved, this, &MainWindow::handleSatResults);
    connect(serialPort, &SerialPort::errorOccurred, this, &MainWindow::handleSpError);

    TimeThread *timeWorker = new TimeThread();
    timeWorker->moveToThread(&timeThread);
    connect(&timeThread, &QThread::finished, timeWorker, &QObject::deleteLater);
    connect(timeWorker, &TimeThread::timeUpdated, this, &MainWindow::handleTimeResults);
    connect(this, &MainWindow::startTimeThread, timeWorker, &TimeThread::updateTime);

    // FlashPicoUf2File *flashUf2Worker = new FlashPicoUf2File();
    // flashUf2Worker->moveToThread(&uf2FlashThread);
    // connect(&uf2FlashThread, &QThread::finished, flashUf2Worker, &QObject::deleteLater);
    // connect(flashUf2Worker, &FlashPicoUf2File::uf2FileFlashed, this, &MainWindow::handleUf2Flashed);
    // connect(flashUf2Worker, &FlashPicoUf2File::errorOccurred, this, &MainWindow::handleUf2Error);

    

    serialPort->openSerialPort();

    timeThread.start();

    emit startTimeThread();
}


void MainWindow::handleSatResults(const QJsonObject &data)
{
    // Access "sensor_data"
    QJsonObject sensorData = data["sensor_data"].toObject();

    // Access "accelerometer"
    QJsonObject accelerometer = sensorData["accelerometer"].toObject();
    int accelXData = accelerometer["accel_x"].toInt();
    int accelYData = accelerometer["accel_y"].toInt();
    int accelZData = accelerometer["accel_z"].toInt();

    // Access Data"gyroscope"
    QJsonObject gyroscope = sensorData["gyroscope"].toObject();
    int gyroXData = gyroscope["gyro_x"].toInt();
    int gyroYData = gyroscope["gyro_y"].toInt();
    int gyroZData = gyroscope["gyro_z"].toInt();

    // Access "environment"
    QJsonObject environment = sensorData["environment"].toObject();
    int altitudeData = environment["altitude (m)"].toInt();
    int humidityData = environment["humidity (%)"].toInt();
    int pressureData = environment["pressure (hPa)"].toInt();
    int temperatureData = environment["temperature (*C)"].toInt();

    // Access "sensor_meta_data"
    QJsonObject metaData = sensorData["sensor_meta_data"].toObject();
    int mpuTemperatureData = metaData["mpu_temperature"].toInt();

    // Access UTC data
    QJsonObject utcData = data["utc_data"].toObject();
    QString utcTime = utcData["UTC time"].toString();

    accelX->setText("Accel. X: " + QString::number(accelXData));
    accelY->setText("Accel. Y: " + QString::number(accelYData));
    accelZ->setText("Accel. Z: " + QString::number(accelZData));

    gyroX->setText("Gyro. X: " + QString::number(gyroXData));
    gyroY->setText("Gyro. Y: " + QString::number(gyroYData));
    gyroZ->setText("Gyro. Z: " + QString::number(gyroZData));

    altitude->setText("Altitude: " + QString::number(altitudeData) + " m");
    humidity->setText("Humidity: " + QString::number(humidityData) + " %");
    pressure->setText("Pressure" + QString::number(pressureData) + " hPa");
    temperature->setText("Temeperature: " + QString::number(temperatureData) + " °C");

    mpuTemperature->setText("MPU Temperature: " + QString::number(mpuTemperatureData) + " °C");

    // Optional: Display UTC time
    utcLabel->setText("UTC time: " + utcTime);
}


void MainWindow::handleSpError(const QString &error)
{
    qStdout() << "[ERROR] MainWindow::handleSpError: " << error << "\n[INFO] Pico most likely not in BOOTSEL mode" << Qt::endl;
    qStdout().flush();
}


void MainWindow::handleTimeResults(const QString &time)
{
    timeLabel->setText(time);
}


void MainWindow::handleUf2Flashed(const QProcess::ExitStatus &status)
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Pico UF2 file flashed");

    QString statusText;
    if (status == QProcess::NormalExit) {
        statusText = "The process exited normally.";
    } else {
        statusText = "The process crashed.";
    }


    qStdout() << "[INFO] MainWindow::handleUf2Flashed: " << statusText << Qt::endl;
    qStdout().flush();

    msgBox.setText(statusText);
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();
}


void MainWindow::handleUf2Error(const QProcess::ProcessError &status) 
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Pico UF2 file flash error");

    QString statusText;
    switch (status) {
        case QProcess::FailedToStart:
            statusText = "The process failed to start.";
            break;
        case QProcess::Crashed:
            statusText = "The process crashed.";
            break;
        case QProcess::Timedout:
            statusText = "The process timed out.";
            break;
        case QProcess::WriteError:
            statusText = "An error occurred when attempting to write to the process.";
            break;
        case QProcess::ReadError:
            statusText = "An error occurred when attempting to read from the process.";
            break;
        case QProcess::UnknownError:
        default:
            statusText = "An unknown error occurred.";
            break;
    }

    qStdout() << "[INFO] MainWindow::handleUf2Error: " << statusText << Qt::endl;
    qStdout().flush();

    msgBox.setText(statusText);
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();
}