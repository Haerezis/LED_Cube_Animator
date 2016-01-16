#ifndef __ANIMATION_CONTROLLER_HPP
#define __ANIMATION_CONTROLLER_HPP

#include <QObject>
#include "AnimationFrameController.hpp"
#include "Animation.hpp"
#include "ui_MainWindows.h"


class AnimationController : public QObject
{
  Q_OBJECT
  public:
    AnimationController(QMainWindow &mainWindow, Animation &animation, AnimationFrameController &controller);
    ~AnimationController() {}

    void setupConnect(Ui::MainWindow &mainWindow);

    bool load();
    bool save();

    void setFrame(unsigned int index);
    void addFrame();

    void hasBeenModified(bool val);

    void setCurrentFrame(unsigned int index);
  public slots:
    bool newAnimation();
    bool openAnimation();
    bool saveAnimation();
    bool saveAnimationAs();
    bool generateData();

  private:
    bool _hasBeenModified;

    QMainWindow &_mainWindow;

    std::string _filepath;

    Animation &_animation;

    AnimationFrameController &_frameController;
};

#endif
