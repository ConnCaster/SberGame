#include "spec_actions/HillSpecAction.h"
#include "interfaces/ICanBeHilled.h"

#include <iostream>


HillSpecAction::HillSpecAction(unsigned int hill_power)
        : max_hill_power_(hill_power)
{}

void HillSpecAction::DoSpecAction(IUnit *target) {
    ICanBeHilled* target_can_be_hilled = dynamic_cast<ICanBeHilled*>(target);
    if (target_can_be_hilled == nullptr) {
        std::cout << "[HillAction] Target does not support Hill Interface" << std::endl;
    } else {
        target_can_be_hilled->Hill(rand()%max_hill_power_);
    }
}

