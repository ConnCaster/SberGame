#ifndef SBERGAME_HILLSPECACTION_H
#define SBERGAME_HILLSPECACTION_H

#include "interfaces/ISpecAction.h"
#include "interfaces/IUnit.h"

class HillSpecAction : public ISpecAction {
public:
    HillSpecAction(unsigned int hill_power);
    ~HillSpecAction() override = default;

    void DoSpecAction(IUnit* target);

private:
    unsigned int max_hill_power_;
};

#endif //SBERGAME_HILLSPECACTION_H
