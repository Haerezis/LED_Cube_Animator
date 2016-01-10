#include <iostream>

#include "ui_MainWindows.h"
#include "ActionsSlot.hpp"
#include <QApplication>
#include <QMainWindow>
#include <QFile>
#include <QTextStream>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  QMainWindow w;

  Ui::MainWindow ui;
  ui.setupUi(&w);


  QFile f(":qdarkstyle/style.qss");
  if (!f.exists())
  {
    std::cerr << "Unable to set stylesheet, file not found"<<std::endl;
  }
  else 
  {
    f.open(QFile::ReadOnly | QFile::Text);
    QTextStream ts(&f);
    a.setStyleSheet(ts.readAll());
  }

  ActionsSlot as;
  as.setupConnect(ui);

  w.show();

  return a.exec();
}
