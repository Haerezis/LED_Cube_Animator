#include "AnimationController.hpp"

#include <string>
#include <fstream>
#include <iostream>
#include <QFileDialog>
#include <QApplication>
#include <QDialog>
#include <QString>
#include <QPushButton>

#include "ui_GenerationOptionsDialog.h"
#include "ui_NewAnimationDialog.h"

AnimationController::AnimationController(QMainWindow &mainWindow, Ui::MainWindow &mainWindowUi) :
  _hasBeenModified(false),
  _mainWindow(mainWindow),
  _duration(*mainWindowUi.duration),
  _animation(3),
  _frame(3),
  _frameController(_frame)
{
  QHeaderView *header = mainWindowUi.frame_list->horizontalHeader();
  header->setSectionResizeMode(QHeaderView::Stretch);
  header->setSectionsClickable(false);
  header->setSectionsMovable(false);
  mainWindowUi.frame_list->setModel(&_frameList);

  QStandardItem *it = nullptr;
  //it = new QStandardItem("ID");
  //_frameList.setHorizontalHeaderItem(0,it);
  it = new QStandardItem("Duration");
  _frameList.setHorizontalHeaderItem(0,it);
}


void AnimationController::setFrame()
{
  unsigned int index = 0;
  auto frames = _animation.frames();
  if(index >= frames.size())
    return;

  frames[index] = _frameController.frame();
  _frameController.clear();
  hasBeenModified(true);
}

void AnimationController::addFrame()
{
  _frameController.frame().duration(_duration.value());
  _frameList.appendRow(new QStandardItem(QString::number(_frameController.frame().duration())));
  _animation.frames().push_back(_frameController.frame());


  _frameController.clear();
  hasBeenModified(true);
}

void AnimationController::setCurrentFrame()
{
  unsigned int index = 0;
  auto frames = _animation.frames();
  if(index >= frames.size())
    return;

  _frameController.frame(frames[index]);
}



void AnimationController::setupConnect(Ui::MainWindow &mainWindow)
{
  _frameController.setupConnect(mainWindow);

  QObject::connect(mainWindow.actionNew_Animation, SIGNAL(triggered()), this, SLOT(newAnimation()));
  QObject::connect(mainWindow.actionOpen_Animation, SIGNAL(triggered()), this, SLOT(openAnimation()));
  QObject::connect(mainWindow.actionSave_Animation, SIGNAL(triggered()), this, SLOT(saveAnimation()));
  QObject::connect(mainWindow.actionSave_Animation_As, SIGNAL(triggered()), this, SLOT(saveAnimationAs()));
  QObject::connect(mainWindow.actionGenerate_C_data, SIGNAL(triggered()), this, SLOT(generateData()));
  QObject::connect(mainWindow.actionQuit, SIGNAL(triggered()), this, SLOT(quitApplication()));

  QObject::connect(mainWindow.add_button, SIGNAL(pressed()), this, SLOT(addFrame()));
  QObject::connect(mainWindow.set_button, SIGNAL(pressed()), this, SLOT(setFrame()));
}

bool AnimationController::load()
{
  if(_filepath.empty())
    return false;

  std::ifstream file(_filepath);
  _animation.load(file);

  if(!_animation.frames().empty())
  {
    _frameController.frame(_animation.frames()[0]);
  }
  
  hasBeenModified(false);
  return true;
}

bool AnimationController::save()
{
  if(_filepath.empty())
    return false;

  std::ofstream file(_filepath);
  _animation.save(file);
  hasBeenModified(false);
  return true;
}


void AnimationController::hasBeenModified(bool val)
{
  _hasBeenModified = val;
  std::string star;
  if(_hasBeenModified) star = "*";

  _mainWindow.setWindowTitle(QString::fromStdString(_filepath + star + " - LED Cube Animator"));
}

bool AnimationController::newAnimation()
{
  Ui::NewAnimationDialog dialogUI;
  QDialog dialog;

  //TODO if _hasBeenModified == true, display dialog to save

  dialogUI.setupUi(&dialog);
  dialogUI.buttonBox->button(QDialogButtonBox::Ok)->setText("Create");

  if(dialog.exec() == QDialog::Accepted)
  {

  }
  hasBeenModified(false);
  return true;
}


bool AnimationController::openAnimation()
{
  QFileDialog fileDialog(NULL, "Open Animation", QDir::home().absolutePath());
  
  //TODO if _hasBeenModified == true, display dialog to save
  
  fileDialog.setFileMode(QFileDialog::AnyFile);

  if(fileDialog.exec() == QDialog::Accepted)
  {
    _filepath = fileDialog.selectedFiles()[0].toStdString();
    load();
  }
  return true;
}


bool AnimationController::saveAnimation()
{
  if(_filepath.empty()) 
    return saveAnimationAs();
  else 
    return save();
}


bool AnimationController::saveAnimationAs()
{
  QFileDialog fileDialog(NULL, "Save Animation", QDir::home().absolutePath());
  
  fileDialog.setFileMode(QFileDialog::AnyFile);

  if(fileDialog.exec() == QDialog::Accepted)
  {
    _filepath = fileDialog.selectedFiles()[0].toStdString();
    save();
  }
  return true;
}


bool AnimationController::generateData()
{
  QFileDialog fileDialog;
  
  fileDialog.setFileMode(QFileDialog::AnyFile);
  fileDialog.setLabelText (QFileDialog::Accept, "Generate");

  if(fileDialog.exec() == QDialog::Accepted)
  {
    Ui::GenerationOptionsDialog dialogUI;
    QDialog dialog;

    dialogUI.setupUi(&dialog);
    if(dialog.exec() == QDialog::Accepted)
    {
      Animation::DataFormat dataFormat = 
        dialogUI.data_format_hexa->isChecked() ?
        Animation::DataFormat::Hexadecimal :
        Animation::DataFormat::Binary;
      unsigned int dataSize = dialogUI.data_size->currentText().toUInt();
      
      std::ofstream file(fileDialog.selectedFiles()[0].toStdString());

      _animation.generate(file, dataFormat, dataSize);
    }
  }
  return true;
}


bool AnimationController::quitApplication()
{
  //TODO Check if current animation is not saved.
  QApplication::quit();
  return true;
}

