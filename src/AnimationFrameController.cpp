#include "AnimationFrameController.hpp"


AnimationFrameController::AnimationFrameController(AnimationFrame &frame) :
  _frame(frame)
{

}

void AnimationFrameController::setupConnect(Ui::MainWindow &mainWindowsUI)
{

}

AnimationFrame AnimationFrameController::frame()
{
  return _frame;
}

void AnimationFrameController::frame(AnimationFrame &frame)
{
  _frame = frame;
}

void AnimationFrameController::clear()
{
  _frame.clear();
}
