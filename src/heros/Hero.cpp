#include <iostream>

#include "heros/Hero.h"
#include "attacks/NormalAttack.h"

constexpr static unsigned int kMaxHealth = 100;
constexpr static unsigned int kMaxProtection = 10;
constexpr static unsigned int kMaxDamage = 20;

Hero::Hero()
    : IUnit(kMaxHealth, kMaxProtection, kMaxDamage),
    attack_{std::make_unique<NormalAttack>()}
{}

Hero::Hero(const Hero& other)
    : IUnit(other.health_, other.protection_, other.damage_), attack_{std::make_unique<NormalAttack>()}
{}

IUnit* Hero::Clone() {
    // TODO: подумать о реализации умных указателей, чтобы не вызывать деструкторы
    Hero* hero = new Hero{*this};
    return hero;
}

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

void Hero::Hill(unsigned int additional_health) {
    health_ = (health_ + additional_health > kMaxHealth) ? kMaxHealth : health_ + additional_health;
}

void Hero::PerformAttack(IUnit *target) {
    if (attack_) {
        attack_->DoAttack(target, damage_);
    } else {
        std::cout << "[Hero] There is no attack set. Abort..." << std::endl;
    }
}