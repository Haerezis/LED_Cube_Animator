#include <iostream>

#include "ui_MainWindows.h"
#include "Animation.hpp"
#include "AnimationFrame.hpp"
#include "AnimationController.hpp"
#include "AnimationFrameController.hpp"
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

  Animation animation(3);
  AnimationFrame frame(3);
  AnimationFrameController frameController(frame);
  AnimationController animationController(w, animation, frameController);

  frameController.setupConnect(ui);
  animationController.setupConnect(ui);

  w.show();

  return a.exec();
}
