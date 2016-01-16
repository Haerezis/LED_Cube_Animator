#ifndef __ACTIONS_SLOT_HPP
#define __ACTIONS_SLOT_HPP

#include <QObject>

#include "ui_MainWindows.h"

class ActionsSlot : public QObject
{
  Q_OBJECT
  public:
    ActionsSlot() {}
    ~ActionsSlot() {}
    void about();
};

#endif
