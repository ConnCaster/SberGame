#include "heros/Hero.h"

constexpr static unsigned int kHealth = 100;
constexpr static unsigned int kProtection = 10;
constexpr static unsigned int kDamage = 20;

Hero::Hero()
    : IUnit(kHealth, kProtection, kDamage), attack_{nullptr}
{}

Hero::Hero(unsigned int health, unsigned int protection, unsigned int damage)
        : IUnit(health, protection, damage), attack_{nullptr}
{}

void Hero::DecreaseHealth(unsigned int damage) {
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

void Hero::SetAttack(std::unique_ptr<IAttack> attack) {
    attack_ = std::move(attack);
}

void Hero::IncreaseHealth(unsigned int additional_health) {
    health_ = (health_ + additional_health > kHealth) ? kHealth : health_ + additional_health;
}

void Hero::PerformAttack(IUnit *target) {
    if (attack_) {
        attack_->DoAttack(target, damage_);
    } else {
        std::cout << "[Hero] There is no attack set. Abort..." << std::endl;
    }
}
