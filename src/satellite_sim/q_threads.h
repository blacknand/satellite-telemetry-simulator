#ifndef Q_THREADS_H
#define Q_THREADS_H

#include "../common/satellite_data.h"
// #include "q_satellite_factory.h" 

#include <QThread>
#include <QObject>
#include <QString>
#include <QTimer>
#include <nlohmann/json.hpp>

using json = nlohmann::json;


class SatDataThread : public QObject {
    Q_OBJECT
public:
    SatDataThread() = default;
    ~SatDataThread() = default;
private:
    // SatFactory satFactory;
public slots:
    void processData();
signals:
    void dataReady(const json &result);
};


class TimeThread : public QObject {
    Q_OBJECT
public:
    TimeThread() = default;
    ~TimeThread() = default;
public slots:
    void updateTime();
signals:
    void timeUpdated(const QString &result);
private:
    void getCurrentTime();
private:
    QTimer *timer;
};

#endif // Q_THREADS_H