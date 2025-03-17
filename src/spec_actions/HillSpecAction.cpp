#include <cstdlib>
#include "spec_actions/HillSpecAction.h"

HillSpecAction::HillSpecAction(unsigned int hill_power)
        : max_hill_power_(hill_power)
{}

void HillSpecAction::DoSpecAction(IUnit *target) {
    if(target->IsHillable()) {
        target->IncreaseHealth(rand()%max_hill_power_);
    }
}

