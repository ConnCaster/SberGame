#ifndef SBERGAME_CLONESPECACTION_H
#define SBERGAME_CLONESPECACTION_H

#include "interfaces/ISpecAction.h"
#include "interfaces/IUnit.h"

class CloneSpecAction : public ISpecAction {
public:
    CloneSpecAction() = default;
    ~CloneSpecAction() override = default;

    void DoSpecAction(IUnit* target);

    IUnit* GetClonedHero() const { return cloned_hero; }
private:
    IUnit* cloned_hero{nullptr};
};

#endif //SBERGAME_CLONESPECACTION_H
