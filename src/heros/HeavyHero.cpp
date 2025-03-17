#include "heros/HeavyHero.h"

constexpr static unsigned int kHealth = 150;
constexpr static unsigned int kProtection = 15;
constexpr static unsigned int kDamage = 25;

HeavyHero::HeavyHero()
        : IUnit(kHealth, kProtection, kDamage), attack_{nullptr}
{}

HeavyHero::HeavyHero(unsigned int health, unsigned int protection, unsigned int damage)
    : IUnit(health, protection, damage), attack_{nullptr}
{}

void HeavyHero::DecreaseHealth(unsigned int damage) {
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

void HeavyHero::SetAttack(std::unique_ptr<IAttack> attack) {
    attack_ = std::move(attack);
}

void HeavyHero::PerformAttack(IUnit *target) {
    if (attack_) {
        attack_->DoAttack(target, damage_);
    } else {
        std::cout << "[HeavyHero] There is no attack set. Abort..." << std::endl;
    }
}
