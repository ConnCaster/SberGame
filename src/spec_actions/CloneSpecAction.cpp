#include "spec_actions/CloneSpecAction.h"

void CloneSpecAction::DoSpecAction(IUnit *target) {
    if(target->GetIsClonable()) {
        cloned_hero = target->Clone();
    }
}