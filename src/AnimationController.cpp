#include "AnimationController.hpp"

#include <string>
#include <fstream>
#include <iostream>
#include <QFileDialog>
#include <QMessageBox>
#include <QApplication>
#include <QDialog>
#include <QString>
#include <QPushButton>

#include "ui_GenerationOptionsDialog.h"
#include "ui_NewAnimationDialog.h"

const unsigned int AnimationController::_defaultCubeSize = 3;

AnimationController::AnimationController(QMainWindow& mainWindow, Ui::MainWindow& mainWindowUi) :
  _modificationNotSaved(false),
  _mainWindow(mainWindow),
  _mainWindowUi(mainWindowUi),
  _duration(*mainWindowUi.duration),
  _cubeOpenGL(*mainWindowUi.cubeOpenGL),
  _animation(_defaultCubeSize)
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
  updateAnimationState(false);

  _cubeOpenGL.setAnimationFrame(_animation.frames()[0]);

  setupConnect();
}


void AnimationController::setSelectedFramesDuration()
{
  auto indexes = _getSelectedFramesIndex();
  unsigned int duration = _duration.value();

  for(auto& index : indexes)
  {
    _animation.frames()[index]->duration(duration);
    _frameList.item(index)->setText(QString::number(duration));
  }

  updateAnimationState(true);
}

void AnimationController::addFrame()
{
  unsigned int duration = _duration.value();

  _animation.frames().emplace_back(new AnimationFrame(_animation.cubeSize(), duration));
  auto item = new QStandardItem(QString::number(duration));
  item->setTextAlignment(Qt::AlignCenter);
  _frameList.appendRow(item);

  _mainWindowUi.frame_list->selectRow(_animation.frames().size() - 1);

  updateAnimationState(true);
}


void AnimationController::setupConnect()
{
  QObject::connect(&_cubeOpenGL, SIGNAL(frameUpdated()), this, SLOT(animationUpdated()));

  QObject::connect(_mainWindowUi.actionNew_Animation, SIGNAL(triggered()), this, SLOT(newAnimation()));
  QObject::connect(_mainWindowUi.actionOpen_Animation, SIGNAL(triggered()), this, SLOT(openAnimation()));
  QObject::connect(_mainWindowUi.actionSave_Animation, SIGNAL(triggered()), this, SLOT(saveAnimation()));
  QObject::connect(_mainWindowUi.actionSave_Animation_As, SIGNAL(triggered()), this, SLOT(saveAnimationAs()));
  QObject::connect(_mainWindowUi.actionGenerate_C_data, SIGNAL(triggered()), this, SLOT(generateData()));
  QObject::connect(_mainWindowUi.actionQuit, SIGNAL(triggered()), this, SLOT(quitApplication()));

  QObject::connect(_mainWindowUi.add_button, SIGNAL(pressed()), this, SLOT(addFrame()));
  QObject::connect(_mainWindowUi.set_button, SIGNAL(pressed()), this, SLOT(setSelectedFramesDuration()));

  QObject::connect(
      _mainWindowUi.frame_list->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
      this, SLOT(frameSelected()));
}

void AnimationController::frameSelected()
{
  auto indexes = _getSelectedFramesIndex();

  if(!indexes.empty())
  {
    unsigned int selectedFrame = indexes.back();
    if(selectedFrame < _animation.frames().size())
    {
      _cubeOpenGL.setAnimationFrame(_animation.frames()[selectedFrame]);
      _cubeOpenGL.update();
    }
  }
}

bool AnimationController::reset(unsigned int cubeSize)
{
  _filepath.clear();

  _animation.frames().clear();
  _frameList.removeRows(0, _frameList.rowCount());

  _animation.cubeSize(cubeSize);

  addFrame();
  
  updateAnimationState(false);
  return true;
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
  {
    auto item = new QStandardItem(QString::number(frame->duration()));
    item->setTextAlignment(Qt::AlignCenter);
    _frameList.appendRow(item);
  }

  _mainWindowUi.frame_list->selectRow(0);
  
  updateAnimationState(false);
  return true;
}

bool AnimationController::save()
{
  if(_filepath.empty())
    return false;

  std::ofstream file(_filepath);
  _animation.save(file);
  updateAnimationState(false);
  return true;
}


void AnimationController::updateAnimationState(bool modificationNotSaved)
{
  _modificationNotSaved = modificationNotSaved;
  std::string star = (modificationNotSaved ? "*" : "");
  _mainWindow.setWindowTitle(QString::fromStdString(_filepath + star + " - LED Cube Animator"));
}

bool AnimationController::newAnimation()
{
  bool retval = true;
  Ui::NewAnimationDialog dialogUI;
  QDialog dialog;

  if (_modificationNotSaved)
  {
    retval = _unsavedModificationDialog(
        "The animation has been modified",
        "Do you want to save your changes before opening another animation ?");
  }

  if(retval)
  {
    dialogUI.setupUi(&dialog);
    dialogUI.buttonBox->button(QDialogButtonBox::Ok)->setText("Create");

    if(dialog.exec() == QDialog::Accepted)
    {
      reset(dialogUI.cube_size->value());
    }
    else
      retval = false;
  }

  return retval;
}


bool AnimationController::openAnimation()
{
  bool retval = true;
  QFileDialog fileDialog(NULL, "Open Animation", QDir::home().absolutePath());
  
  if (_modificationNotSaved)
  {
    retval = _unsavedModificationDialog(
        "The animation has been modified",
        "Do you want to save your changes before opening another animation ?");
  }

  if(retval)
  {
    fileDialog.setFileMode(QFileDialog::AnyFile);
    if(fileDialog.exec() == QDialog::Accepted)
    {
      _filepath = fileDialog.selectedFiles()[0].toStdString();
      load();
    }
    else
    {
      retval = false;
    }
  }
  
  return retval;
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
  bool retval = false;
  QFileDialog fileDialog(NULL, "Save animation as..", QDir::home().absolutePath());
  
  fileDialog.setFileMode(QFileDialog::AnyFile);

  if(fileDialog.exec() == QDialog::Accepted)
  {
    _filepath = fileDialog.selectedFiles()[0].toStdString();
    save();
    retval = true;
  }
  return retval;
}


bool AnimationController::generateData()
{
  QFileDialog fileDialog;
  
  fileDialog.setFileMode(QFileDialog::AnyFile);
  fileDialog.setLabelText (QFileDialog::Accept, "Generate");

  if(fileDialog.exec() == QDialog::Accepted)
  {
    std::ofstream file(fileDialog.selectedFiles()[0].toStdString());
    _animation.generate(file);
  }
  return true;
}

std::vector<unsigned int> AnimationController::_getSelectedFramesIndex()
{
  std::vector<unsigned int> retval;

  for(const auto& elt : _mainWindowUi.frame_list->selectionModel()->selectedIndexes())
  {
    retval.push_back(elt.row());
  }

  return retval;
}

bool AnimationController::_unsavedModificationDialog(const std::string& text, const std::string& informativeText)
{
  if (!_modificationNotSaved)
    return false;

  bool retval = true;

  QMessageBox msgBox;
  msgBox.setText(QString::fromStdString(text));
  msgBox.setInformativeText(QString::fromStdString(informativeText));
  msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
  msgBox.setDefaultButton(QMessageBox::Save);
  int ret = msgBox.exec();

  if(ret == QMessageBox::Cancel)
  {
    retval = false;
  }
  else if(ret == QMessageBox::Save)
  {
    retval = saveAnimation();
  }

  return retval;
}

bool AnimationController::quitApplication()
{
  bool retval = true;
  if (_modificationNotSaved)
  {
    retval = _unsavedModificationDialog(
        "The animation has been modified",
        "Do you want to save your changes before exiting the program ?");
  }

  if(retval)
    QApplication::quit();

  return retval;
}
