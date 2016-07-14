#ifndef __ANIMATION_CONTROLLER_HPP
#define __ANIMATION_CONTROLLER_HPP

#include <QObject>
#include <QStandardItemModel>
#include "AnimationFrameController.hpp"
#include "Animation.hpp"
#include "CubeOpenGL.hpp"
#include "ui_MainWindows.h"


class AnimationController : public QObject
{
  Q_OBJECT
  public:
    AnimationController(QMainWindow &mainWindow, Ui::MainWindow &mainWindowUi);
    ~AnimationController() {}

    void setupConnect();

    bool load();
    bool save();


    void hasBeenModified(bool val);

  public slots:
    void addFrame();
    void setFrame();
    void setCurrentFrame();
    bool newAnimation();
    bool openAnimation();
    bool saveAnimation();
    bool saveAnimationAs();
    bool generateData();
    bool quitApplication();
    void frameSelected();

  private:
    bool _hasBeenModified;

    QMainWindow& _mainWindow;
    Ui::MainWindow& _mainWindowUi;
    QSpinBox& _duration;
    CubeOpenGL& _cubeOpenGL;

    std::string _filepath;

    Animation _animation;
    QStandardItemModel _frameList;
};

#endif
