#include "ui_MainWindows.h"
#include <QApplication>
#include <QMainWindow>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QMainWindow w;
    Ui_MainWindow ui;
    ui.setupUi(&w);
    w.show();

    return a.exec();
}
