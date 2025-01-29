#include "../data_preprocessing/json_conversion.h"
#include "q_threads.h"
#include "mainwindow.h"

#include <QTimer>
#include <QDateTime>
#include <QTime>


void SatDataThread::processData()
{
    // SatelliteData satelliteData = satFactory.get_satellite_data();
    // json j = satelliteData;
    // emit dataReady(j);
}


void TimeThread::updateTime()
{
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &TimeThread::getCurrentTime);
    timer->start(1000);
}


void TimeThread::getCurrentTime()
{
    QDateTime current = QDateTime::currentDateTime();
    QString time = current.toString("hh:mm:ss");
    emit timeUpdated(time);
}