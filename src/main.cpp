#include <iostream>
#include <cstdlib>

#include "ui_MainWindows.h"
#include "Animation.hpp"
#include "AnimationFrame.hpp"
#include "AnimationController.hpp"
#include "AnimationFrameController.hpp"
#include <QApplication>
#include <QMainWindow>
#include <QFile>
#include <QTextStream>
#include <QStringListModel>

int main(int argc, char *argv[])
{
  srand (time(NULL));

  QApplication a(argc, argv);
  QMainWindow w;
  QStringListModel frames;

  Ui::MainWindow ui;
  ui.setupUi(&w);
  ui.frame_list->setModel(&frames);


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

  AnimationController animationController(w, ui);

  w.show();

  return a.exec();
}
