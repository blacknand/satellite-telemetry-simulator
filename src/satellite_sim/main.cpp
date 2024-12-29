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

    MainWindow w;

    // Start application only if plugins are available
    if (!w.hasPlugins()) {
        QMessageBox::critical(nullptr,
                              "No viewer plugins found"_L1,
                              "Unable to load viewer plugins. Exiting application."_L1);
        return 1;
    }

    w.show();
    if (!fileName.isEmpty())
        w.openFile(fileName);

    return app.exec();
}