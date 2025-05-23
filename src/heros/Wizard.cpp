#include <iostream>

#include "heros/Wizard.h"
#include "spec_actions/CloneSpecAction.h"

constexpr static unsigned int kMaxHealth = 70;
constexpr static unsigned int kMaxProtection = 0;
constexpr static unsigned int kMaxDamage = 0;

constexpr static unsigned int kDistance = 1;

Wizard::Wizard()
        : IUnit(kMaxHealth, kMaxProtection, kMaxDamage),
          distance_{kDistance},
          spec_action_{std::make_unique<CloneSpecAction>()}
{}

void Wizard::DecreaseHealth(unsigned int damage) {
    std::cout << "[Wizard] health " << health_ << " -> ";
    if (damage >= health_ + protection_) {
        health_ = 0;
    } else {
        if (damage >= protection_) {
            damage -= protection_;
            health_ -= damage;
        }
    }
    std::cout << health_ << std::endl;
}

void Wizard::SetSpecAction(std::unique_ptr<ISpecAction> spec_action) {
    if (spec_action_ != nullptr) {
        spec_action_.reset(spec_action.get());
    } else {
        spec_action_ = std::move(spec_action);
    }
}

IUnit* Wizard::PerformSpecAction(IUnit *target) {
    if (spec_action_) {
        spec_action_->DoSpecAction(target);

        return dynamic_cast<CloneSpecAction*>(spec_action_.get())->GetClonedHero();
    } else {
        std::cout << "[Hiller] There is no special actions set. Abort..." << std::endl;
        return nullptr;
    }
}
