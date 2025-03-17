#include "heros/Archer.h"

constexpr static unsigned int kHealth = 70;
constexpr static unsigned int kProtection = 10;
constexpr static unsigned int kDamage = 20;

constexpr static unsigned int kDistance = 2;

Archer::Archer()
        : IUnit(kHealth, kProtection, kDamage), distance_{kDistance}, attack_{nullptr}
{}

Archer::Archer(unsigned int health, unsigned int protection, unsigned int damage, unsigned int distance)
        : IUnit(health, protection, damage), distance_{distance}, attack_{nullptr}
{}

void Archer::DecreaseHealth(unsigned int damage) {
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
void Archer::SetAttack(std::unique_ptr<IAttack> attack) {
    attack_ = std::move(attack);
}

void Archer::IncreaseHealth(unsigned int additional_health) {
    health_ = (health_ + additional_health > kHealth) ? kHealth : health_ + additional_health;
}

void Archer::PerformAttack(IUnit *target) {
    if (attack_) {
        attack_->DoAttack(target, damage_);
    } else {
        std::cout << "[Archer] There is no attack set. Abort..." << std::endl;
    }
}