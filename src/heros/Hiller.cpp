#include "heros/Hiller.h"

#include "attacks/NormalAttack.h"
#include "spec_actions/HillSpecAction.h"

constexpr static unsigned int kMaxHealth = 70;
constexpr static unsigned int kMaxProtection = 10;
constexpr static unsigned int kMaxDamage = 20;

constexpr static bool kIsHillable = false;
constexpr static bool kIsClonable = true;

constexpr static unsigned int kDistance = 2;
constexpr static unsigned int kMaxPower = 30;

Hiller::Hiller()
        : IUnit(kMaxHealth, kMaxProtection, kMaxDamage, kIsHillable, kIsClonable),
        distance_{kDistance},
        attack_{std::make_unique<NormalAttack>()},
        spec_action_{std::make_unique<HillSpecAction>(kMaxPower)}
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
    if (attack_ != nullptr) {
        attack_.reset(attack.get());
    } else {
        attack_ = std::move(attack);
    }
}

void Hiller::SetSpecAction(std::unique_ptr<ISpecAction> spec_action) {
    if (spec_action_ != nullptr) {
        spec_action_.reset(spec_action.get());
    } else {
        spec_action_ = std::move(spec_action);
    }
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
