#include "mainwindow.h"
#include "serial_port.h"

#include <QApplication>
#include <QCommandLineParser>
#include <QMainWindow>
#include <QPlainTextEdit>


int main(int argc, char* argv[]) {
    QApplication satellite_sim(argc, argv);
    QCoreApplication::setOrganizationName("blacknand");
    QCoreApplication::setApplicationName("SatelliteSim");
    QCoreApplication::setApplicationVersion("0.1");

    QCommandLineParser parser;
    parser.setApplicationDescription(QApplication::translate("main", "A satellite telemetry simulator."));
    parser.addHelpOption();
    parser.addVersionOption();
    parser.process(satellite_sim);

    MainWindow window;
    window.show();

    return satellite_sim.exec();
}