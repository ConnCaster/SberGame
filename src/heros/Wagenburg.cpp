#include <iostream>

#include "heros/Wagenburg.h"

constexpr unsigned int kMaxHealth = 200;

Wagenburg::Wagenburg()
    : health_{kMaxHealth}, protection_{0}, damage_{0}
{}

void Wagenburg::GetShoot(unsigned int damage) {
    std::cout << "[Wagenburg] health " << health_ << " -> ";
    health_ = (damage > health_) ? 0 : health_ - damage;
    std::cout << health_ << std::endl;
}

