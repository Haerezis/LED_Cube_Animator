#ifndef __ACTIONS_SLOT_HPP
#define __ACTIONS_SLOT_HPP

#include <QObject>

class ActionsSlot : public QObject
{
  Q_OBJECT
  public:
    ActionsSlot() {}
    ~ActionsSlot() {}
    void about();
};

#endif
