#include "ActionsSlot.hpp"

#include <QFileDialog>
#include <QApplication>
#include <QDialog>
#include <QPushButton>
#include <QMessageBox>

#include "ui_GenerationOptionsDialog.h"
#include "ui_NewAnimationDialog.h"

void ActionsSlot::setupConnect(Ui::MainWindow &mainWindowsUI)
{
  QObject::connect(mainWindowsUI.actionNew_Animation, SIGNAL(triggered()), this, SLOT(newAnimation()));
  QObject::connect(mainWindowsUI.actionOpen_Animation, SIGNAL(triggered()), this, SLOT(openAnimation()));
  QObject::connect(mainWindowsUI.actionSave_Animation, SIGNAL(triggered()), this, SLOT(saveAnimation()));
  QObject::connect(mainWindowsUI.actionSave_Animation_As, SIGNAL(triggered()), this, SLOT(saveAnimationAs()));
  QObject::connect(mainWindowsUI.actionGenerate_C_data, SIGNAL(triggered()), this, SLOT(generateData()));
  QObject::connect(mainWindowsUI.actionAbout_this_application, SIGNAL(triggered()), this, SLOT(about()));
}


bool ActionsSlot::newAnimation()
{
  Ui::NewAnimationDialog dialogUI;
  QDialog dialog;

  dialogUI.setupUi(&dialog);
  dialogUI.buttonBox->button(QDialogButtonBox::Ok)->setText("Create");

  if(dialog.exec() == QDialog::Accepted)
  {

  }
  return true;
}


bool ActionsSlot::openAnimation()
{
  QFileDialog fileDialog;
  
  fileDialog.setFileMode(QFileDialog::AnyFile);

  if(fileDialog.exec() == QDialog::Accepted)
  {
  }

  return true;
}


bool ActionsSlot::saveAnimation()
{
  return true;
}


bool ActionsSlot::saveAnimationAs()
{
  QFileDialog fileDialog;
  
  fileDialog.setFileMode(QFileDialog::AnyFile);

  if(fileDialog.exec() == QDialog::Accepted)
  {
  }

  return true;
}


bool ActionsSlot::generateData()
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

    }

  }

  return true;
}

void ActionsSlot::about()
{
  QMessageBox::about(
      NULL,
      "About LED Cube Animator",
      "LED Cube Animator is released under the MIT licence.\
You can find the source at <a href='https://github.com/Haerezis/LED_Cube_Animator'>this link</a> .");
}
