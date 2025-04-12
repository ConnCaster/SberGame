#include <iostream>
#include <attacks/DistanceAttack.h>

#include "heros/Archer.h"

constexpr static unsigned int kMaxHealth = 70;
constexpr static unsigned int kMaxProtection = 10;
constexpr static unsigned int kMaxDamage = 20;

constexpr static unsigned int kDistance = 2;

unsigned int Archer::distance_ = kDistance;

Archer::Archer()
        : IUnit(kMaxHealth, kMaxProtection, kMaxDamage),
        attack_{std::make_unique<DistanceAttack>()}
{}

Archer::Archer(const Archer& other)
    : IUnit(other.health_, other.protection_, other.damage_), attack_{std::make_unique<DistanceAttack>()}
{}

IUnit* Archer::Clone() {
    Archer* archer = new Archer{*this};
    return archer;
}

void Archer::DecreaseHealth(unsigned int damage) {
    std::cout << "[Archer] health " << health_ << " -> ";
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

void Archer::SetAttack(std::unique_ptr<IAttack> attack) {
    if (attack_ != nullptr) {
        attack_.reset(attack.get());
    } else {
        attack_ = std::move(attack);
    }
}

void Archer::Hill(unsigned int additional_health) {
    std::cout << "[Archer] health " << health_ << " -> ";
    health_ = (health_ + additional_health > kMaxHealth) ? kMaxHealth : health_ + additional_health;
    std::cout << health_ << std::endl;
}

void Archer::PerformAttack(IUnit *target) {
    if (attack_) {
        attack_->DoAttack(target, damage_);
    } else {
        std::cout << "[Archer] There is no attack set. Abort..." << std::endl;
    }
}