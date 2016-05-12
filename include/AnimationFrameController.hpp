#ifndef __ANIMATION_FRAME_CONTROLLER_HPP
#define __ANIMATION_FRAME_CONTROLLER_HPP

#include "ui_MainWindows.h"
#include "AnimationFrame.hpp"
#include <QObject>

class AnimationFrameController : public QObject
{
  Q_OBJECT
  public:
    AnimationFrameController(AnimationFrame &frame);
    ~AnimationFrameController() {}

    void setupConnect(Ui::MainWindow &mainWindowsUI);

    AnimationFrame& frame();
    void frame(AnimationFrame &frame);
    void clear();
  private:
    AnimationFrame _frame;
};

#endif
