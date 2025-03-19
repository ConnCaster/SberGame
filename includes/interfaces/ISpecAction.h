#ifndef SBERGAME_ISPECACTION_H
#define SBERGAME_ISPECACTION_H

#include <vector>

#include "interfaces/IUnit.h"

class ISpecAction {
public:
    virtual void DoSpecAction() {};
    virtual void DoSpecAction(IUnit* target) {};
    virtual void DoSpecAction(std::vector<IUnit*>& targets) {};
    virtual ~ISpecAction() = default;
};

#endif //SBERGAME_ISPECACTION_H
