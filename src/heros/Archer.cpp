#include <iostream>

#include "heros/Archer.h"

constexpr static unsigned int kMaxHealth = 70;
constexpr static unsigned int kMaxProtection = 10;
constexpr static unsigned int kMaxDamage = 20;

constexpr static unsigned int kDistance = 2;

Archer::Archer()
        : IUnit(kMaxHealth, kMaxProtection, kMaxDamage),
        distance_{kDistance},
        attack_{nullptr}
{}

Archer::Archer(const Archer& other)
    : IUnit(other.health_, other.protection_, other.damage_), attack_{nullptr}
{}

IUnit* Archer::Clone() {
    // TODO: подумать о реализации умных указателей, чтобы не вызывать деструкторы
    Archer* archer = new Archer{*this};
    return archer;
}

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
    if (attack_ != nullptr) {
        attack_.reset(attack.get());
    } else {
        attack_ = std::move(attack);
    }
}

void Archer::Hill(unsigned int additional_health) {
    health_ = (health_ + additional_health > kMaxHealth) ? kMaxHealth : health_ + additional_health;
}

void Archer::PerformAttack(IUnit *target) {
    if (attack_) {
        attack_->DoAttack(target, damage_);
    } else {
        std::cout << "[Archer] There is no attack set. Abort..." << std::endl;
    }
}