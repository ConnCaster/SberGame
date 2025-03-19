#include "heros/HeavyHero.h"

constexpr static unsigned int kMaxHealth = 150;
constexpr static unsigned int kMaxProtection = 15;
constexpr static unsigned int kMaxDamage = 25;

constexpr static bool kIsHillable = false;
constexpr static bool kIsClonable = false;

HeavyHero::HeavyHero()
        : IUnit(kMaxHealth, kMaxProtection, kMaxDamage, kIsHillable, kIsClonable),
        attack_{nullptr}
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
    if (attack_ != nullptr) {
        attack_.reset(attack.get());
    } else {
        attack_ = std::move(attack);
    }
}

void HeavyHero::PerformAttack(IUnit *target) {
    if (attack_) {
        attack_->DoAttack(target, damage_);
    } else {
        std::cout << "[HeavyHero] There is no attack set. Abort..." << std::endl;
    }
}
