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
    // No positional arguments for now
    // parser.addPositionalArgument("config"_L1, QApplication::translate("main", "The configuration file to use."_L1));
    parser.process(satellite_sim);

    const QStringList &positionalArguments = parser.positionalArguments();
    const QString &fileName = (positionalArguments.count() > 0) ? positionalArguments.at(0)
                                                                : QString();
    QMainWindow mainWindow;
    QPlainTextEdit *textEdit = new QPlainTextEdit(&mainWindow);
    textEdit->setPlainText("Hello, World!");

    mainWindow.setWindowTitle(QApplication::translate("main", "SatelliteSim"));
    mainWindow.setCentralWidget(textEdit);
    mainWindow.resize(400, 300);
    mainWindow.show();

    return satellite_sim.exec();
}