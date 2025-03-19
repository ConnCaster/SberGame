#include "heros/Hero.h"

constexpr static unsigned int kMaxHealth = 100;
constexpr static unsigned int kMaxProtection = 10;
constexpr static unsigned int kMaxDamage = 20;

constexpr static bool kIsHillable = true;
constexpr static bool kIsClonable = true;

Hero::Hero()
    : IUnit(kMaxHealth, kMaxProtection, kMaxDamage,  kIsHillable,kIsClonable),
    attack_{nullptr}
{}

//IUnit* Hero::Clone() {
//    return nullptr;
//}


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
    if (attack_ != nullptr) {
        attack_.reset(attack.get());
    } else {
        attack_ = std::move(attack);
    }
}

void Hero::IncreaseHealth(unsigned int additional_health) {
    health_ = (health_ + additional_health > kMaxHealth) ? kMaxHealth : health_ + additional_health;
}

void Hero::PerformAttack(IUnit *target) {
    if (attack_) {
        attack_->DoAttack(target, damage_);
    } else {
        std::cout << "[Hero] There is no attack set. Abort..." << std::endl;
    }
}