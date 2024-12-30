#include <QApplication>
#include <QCommandLineParser>
#include <QMainWindow>
#include <QPlainTextEdit>


int main(int argc, char* argv[]) {
    QApplication satellite_tim(argc, argv);
    QCoreApplication::setOrganizationName("blacknand"_L1);
    QCoreApplication::setApplicationName("SatelliteSim"_L1);
    QCoreApplication::setApplicationVersion("0.1"_L1);

    QCommandLineParser parser;
    parser.setApplicationDescription(QApplication::translate("main", "A satellite telemetry simulator."));
    parser.addHelpOption();
    parser.addVersionOption();
    // No positional arguments for now
    // parser.addPositionalArgument("config"_L1, QApplication::translate("main", "The configuration file to use."_L1));
    parser.process(satellite_tim);

    const QStringList &positionalArguments = parser.positionalArguments();
    const QString &fileName = (positionalArguments.count() > 0) ? positionalArguments.at(0)
                                                                : QString();
    QMainWindow mainWindow;
    QPlainTextEdit *textEdit = new QPlainTextEdit(&mainWindow);
    textEdit->setPlainText("Hello, World!");

    mainWindow.setWindowTitle(QApplication::translate("main", "SatelliteSim"_L1));
    mainWindow.setCentralWidget(textEdit);
    mainWindow.resize(400, 300);
    mainWindow.show();

    return app.exec();
}