#ifndef SBERGAME_ISPECACTION_H
#define SBERGAME_ISPECACTION_H

#include "interfaces/IAction.h"

class ISpecAction: public IAction {
public:
    virtual void DoAction() = 0;
};

#endif //SBERGAME_ISPECACTION_H
