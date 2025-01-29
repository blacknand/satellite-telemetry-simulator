#ifndef PICO_FLASHER_H
#define PICO_FLASHER_H

#include <QProcess>
#include <QThread>
#include <QString>


class FlashPicoUf2File : public QThread {
    Q_OBJECT
public:
    FlashPicoUf2File() = default;
    ~FlashPicoUf2File() = default; 
    void flashUf2File();
private:
    QProcess process;
public slots:
    void handleUf2FileFlashed(const QProcess::ExitStatus &status);
    void handleUf2FileError(const QProcess::ProcessError &error);
signals:
    void uf2FileFlashed(const QProcess::ExitStatus &status);
    void errorOccurred(const QProcess::ProcessError &error);   
};

#endif // PICO_FLASHER_H