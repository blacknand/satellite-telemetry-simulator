#include "mainwindow.h"
#include "serial_port.h"

#include <QMainWindow>
#include <QPlainTextEdit>


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    QPlainTextEdit *textEdit = new QPlainTextEdit;
    textEdit->setPlainText("Hello, World!");
    setCentralWidget(textEdit);
    resize(400, 300);
    setWindowTitle("Satellite Telemetry Simulator");
}