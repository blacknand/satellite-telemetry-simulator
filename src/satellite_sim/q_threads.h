#include "../common/satellite_data.h"

#include <QThread>
#include <QObject>
#include <QString>
#include <nlohmann/json.hpp>

using json = nlohmann::json;


class SatDataThread : public QObject {
    Q_OBJECT
public:
    SatDataThread() = default;
    ~SatDataThread() = default;
public slots:
    void processData(const SatelliteInterface &data);
signals:
    void dataReady(const QString &result);
private:
    SatelliteInterface *satelliteInterface = new SatelliteInterface();
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
};