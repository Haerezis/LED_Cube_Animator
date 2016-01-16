#include "ActionsSlot.hpp"

#include <QMessageBox>

void ActionsSlot::about()
{
  QMessageBox::about(
      NULL,
      "About LED Cube Animator",
      "LED Cube Animator is released under the MIT licence.\
You can find the source at <a href='https://github.com/Haerezis/LED_Cube_Animator'>this link</a> .");
}
