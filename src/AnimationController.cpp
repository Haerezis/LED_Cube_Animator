#include "AnimationController.hpp"

#include <fstream>
#include <iostream>
#include <QFileDialog>
#include <QApplication>
#include <QDialog>
#include <QString>
#include <QPushButton>

#include "ui_GenerationOptionsDialog.h"
#include "ui_NewAnimationDialog.h"

AnimationController::AnimationController(QMainWindow &mainWindow, Animation &animation, AnimationFrameController &controller) :
  _hasBeenModified(false),
  _mainWindow(mainWindow),
  _animation(animation),
  _frameController(controller)
{

}


void AnimationController::setFrame(unsigned int index)
{
  auto frames = _animation.frames();
  if(index >= frames.size())
    return;

  frames[index] = _frameController.frame();
  _frameController.clear();
  hasBeenModified(true);
}

void AnimationController::addFrame()
{
  _animation.frames().push_back(_frameController.frame());
  _frameController.clear();
  hasBeenModified(true);
}

void AnimationController::setCurrentFrame(unsigned int index)
{
  auto frames = _animation.frames();
  if(index >= frames.size())
    return;

  _frameController.frame(frames[index]);
}



void AnimationController::setupConnect(Ui::MainWindow &mainWindow)
{
  QObject::connect(mainWindow.actionNew_Animation, SIGNAL(triggered()), this, SLOT(newAnimation()));
  QObject::connect(mainWindow.actionOpen_Animation, SIGNAL(triggered()), this, SLOT(openAnimation()));
  QObject::connect(mainWindow.actionSave_Animation, SIGNAL(triggered()), this, SLOT(saveAnimation()));
  QObject::connect(mainWindow.actionSave_Animation_As, SIGNAL(triggered()), this, SLOT(saveAnimationAs()));
  QObject::connect(mainWindow.actionGenerate_C_data, SIGNAL(triggered()), this, SLOT(generateData()));
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

