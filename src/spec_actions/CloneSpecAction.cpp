#include "spec_actions/CloneSpecAction.h"
#include "interfaces/ICanBeCloned.h"

#include <iostream>

void CloneSpecAction::DoSpecAction(IUnit *target) {
    ICanBeCloned* target_can_be_hilled = dynamic_cast<ICanBeCloned*>(target);
    if (target_can_be_hilled == nullptr) {
        std::cout << "[CloneAction] Target does not support Clone Interface" << std::endl;
    } else {
        cloned_hero = target_can_be_hilled->Clone();
    }
}