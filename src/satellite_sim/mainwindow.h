#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPlainTextEdit>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    putData(const QByteArray &data);
private:
    QPlainTextEdit *textEdit;
};

#endif // MAINWINDOW_H
