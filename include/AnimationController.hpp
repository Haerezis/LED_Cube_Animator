#ifndef __ANIMATION_CONTROLLER_HPP
#define __ANIMATION_CONTROLLER_HPP

#include <QObject>
#include <QStandardItemModel>
#include "AnimationFrameController.hpp"
#include "Animation.hpp"
#include "ui_MainWindows.h"


class AnimationController : public QObject
{
  Q_OBJECT
  public:
    AnimationController(QMainWindow &mainWindow, Ui::MainWindow &mainWindowUi);
    ~AnimationController() {}

    void setupConnect(Ui::MainWindow &mainWindow);

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

  private:
    bool _hasBeenModified;

    QMainWindow &_mainWindow;
    QSpinBox &_duration;

    std::string _filepath;

    Animation _animation;
    QStandardItemModel _frameList;

    AnimationFrame _frame;
    AnimationFrameController _frameController;

};

#endif
