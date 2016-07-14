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

AnimationController::AnimationController(QMainWindow& mainWindow, Ui::MainWindow& mainWindowUi) :
  _hasBeenModified(false),
  _mainWindow(mainWindow),
  _mainWindowUi(mainWindowUi),
  _duration(*mainWindowUi.duration),
  _cubeOpenGL(*mainWindowUi.cubeOpenGL),
  _animation(3)
{
  QHeaderView *header = mainWindowUi.frame_list->horizontalHeader();
  header->setSectionResizeMode(QHeaderView::Stretch);
  header->setSectionsClickable(false);
  header->setSectionsMovable(false);
  mainWindowUi.frame_list->setModel(&_frameList);


  QStandardItem *it = nullptr;
  it = new QStandardItem("ID");
  _frameList.setHorizontalHeaderItem(0,it);
  it = new QStandardItem("Duration");
  _frameList.setHorizontalHeaderItem(0,it);

  //Add initial frame
  addFrame();

  _cubeOpenGL.setAnimationFrame(_animation.frames()[0]);

  setupConnect();
}


void AnimationController::setFrame()
{
  unsigned int index = 0;
  //TODO get selected frames from list.
  auto frames = _animation.frames();
  if(index >= frames.size())
    return;

  //TODO
  hasBeenModified(true);
}

void AnimationController::addFrame()
{
  unsigned int duration = _duration.value();

  _animation.frames().emplace_back(new AnimationFrame(_animation.cubeSize(), duration));
  _frameList.appendRow(new QStandardItem(QString::number(duration)));

  _mainWindowUi.frame_list->selectRow(_animation.frames().size() - 1);

  hasBeenModified(true);
}

void AnimationController::setCurrentFrame()
{
  unsigned int index = 0;
  auto frames = _animation.frames();
  if(index >= frames.size())
    return;

  //TODO
  _cubeOpenGL.update();
}



void AnimationController::setupConnect()
{
  QObject::connect(_mainWindowUi.actionNew_Animation, SIGNAL(triggered()), this, SLOT(newAnimation()));
  QObject::connect(_mainWindowUi.actionOpen_Animation, SIGNAL(triggered()), this, SLOT(openAnimation()));
  QObject::connect(_mainWindowUi.actionSave_Animation, SIGNAL(triggered()), this, SLOT(saveAnimation()));
  QObject::connect(_mainWindowUi.actionSave_Animation_As, SIGNAL(triggered()), this, SLOT(saveAnimationAs()));
  QObject::connect(_mainWindowUi.actionGenerate_C_data, SIGNAL(triggered()), this, SLOT(generateData()));
  QObject::connect(_mainWindowUi.actionQuit, SIGNAL(triggered()), this, SLOT(quitApplication()));

  QObject::connect(_mainWindowUi.add_button, SIGNAL(pressed()), this, SLOT(addFrame()));
  QObject::connect(_mainWindowUi.set_button, SIGNAL(pressed()), this, SLOT(setFrame()));

  QObject::connect(
      _mainWindowUi.frame_list->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
      this, SLOT(frameSelected()));
}

void AnimationController::frameSelected()
{
  auto indexes = _mainWindowUi.frame_list->selectionModel()->selectedIndexes();

  if(!indexes.empty())
  {
    int selectedFrame = indexes.back().row();
    if((0 <= selectedFrame) && (static_cast<unsigned int>(selectedFrame) < _animation.frames().size()))
    {
      _cubeOpenGL.setAnimationFrame(_animation.frames()[selectedFrame]);
      _cubeOpenGL.update();
    }
  }
}

bool AnimationController::load()
{
  if(_filepath.empty())
    return false;

  std::ifstream file(_filepath);

  _animation.load(file);
  _frameList.removeRows(0, _frameList.rowCount());

  if(_animation.frames().empty())
    addFrame();

  for(auto& frame : _animation.frames())
    _frameList.appendRow(new QStandardItem(QString::number(frame->duration())));

  _mainWindowUi.frame_list->selectRow(0);
  
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
