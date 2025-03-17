#include "heros/Hiller.h"

constexpr static unsigned int kHealth = 70;
constexpr static unsigned int kProtection = 10;
constexpr static unsigned int kDamage = 20;

constexpr static unsigned int kDistance = 2;
constexpr static unsigned int kMaxPower = 30;

Hiller::Hiller()
        : IUnit(kHealth, kProtection, kDamage), distance_{kDistance}, attack_{nullptr}, spec_action_{nullptr}
{}

Hiller::Hiller(unsigned int health, unsigned int protection, unsigned int damage, unsigned int distance)
        : IUnit(health, protection, damage), distance_{distance}, attack_{nullptr}, spec_action_{nullptr}
{}

void Hiller::DecreaseHealth(unsigned int damage) {
    if (damage >= health_ + protection_) {
        protection_ = 0;
        health_ = 0;
    } else {
        if (damage >= protection_) {
            damage -= protection_;
            protection_ = 0;
            health_ -= damage;
        } else {
            protection_ -= damage;
        }
    }
}

void Hiller::SetAttack(std::unique_ptr<IAttack> attack) {
    attack_ = std::move(attack);
}

void Hiller::SetSpecAction(std::unique_ptr<ISpecAction> spec_action) {
    spec_action_ = std::move(spec_action);
}

void Hiller::PerformAttack(IUnit *target) {
    if (attack_) {
        attack_->DoAttack(target, damage_);
    } else {
        std::cout << "There is no attack set. Abort..." << std::endl;
    }
}

void Hiller::PerformSpecAction(IUnit *target) {
    if (spec_action_) {
        spec_action_->DoSpecAction(target);
    } else {
        std::cout << "[Hiller] There is no special actions set. Abort..." << std::endl;
    }
}
