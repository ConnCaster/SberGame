#include <iostream>
#include <unordered_map>

#include "heros/HeavyHero.h"
#include "attacks/NormalAttack.h"

constexpr static unsigned int kMaxHealth = 150;
constexpr static unsigned int kMaxProtection = 15;
constexpr static unsigned int kMaxDamage = 25;

HeavyHero::HeavyHero()
        : IUnit(kMaxHealth, kMaxProtection, kMaxDamage),
        attack_{std::make_unique<NormalAttack>()}
{}


void HeavyHero::DecreaseHealth(unsigned int damage) {
    std::cout << "[HeavyHero] health " << health_ << " -> ";
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

std::string ExtractHeavyHeroTypeFromPtr(IUnit* unit) {
    if (typeid(*unit) == typeid(HorseDecorator)) return "HorseDecorator";
    if (typeid(*unit) == typeid(SpearDecorator)) return "SpearDecorator";
    if (typeid(*unit) == typeid(ShieldDecorator)) return "ShieldDecorator";
    if (typeid(*unit) == typeid(HelmetDecorator)) return "HelmetDecorator";
}

HeavyHero* AppendBuffToHeavyHero(HeavyHero* heavy_hero) {
    if (heavy_hero->HasBuff("Horse") &&
            heavy_hero->HasBuff("Spear") &&
            heavy_hero->HasBuff("Shield") &&
            heavy_hero->HasBuff("Helmet")) {
            std::cout << "[Buff] Too much buffs" << std::endl;
            return heavy_hero;
    }
    int buff_counter = rand() % 5;
    while (buff_counter-- != 0) {
        int buff_type = rand() % 4;
        switch (buff_type) {
            case 0: {   // Horse
                if (!heavy_hero->HasBuff("Horse")) {
                    heavy_hero = new HorseDecorator(heavy_hero);
                    std::cout << "[Buff] Append Horse to HeavyHero" << std::endl;
                } else {
                    std::cout << "[Buff] There is Horse already with HeavyHero" << std::endl;
                }
                break;
            }
            case 1: {   // Spear
                if (!heavy_hero->HasBuff("Spear")) {
                    heavy_hero = new SpearDecorator(heavy_hero);
                    std::cout << "[Buff] Append Spear to HeavyHero" << std::endl;
                } else {
                    std::cout << "[Buff] There is Spear already with HeavyHero" << std::endl;
                }
                break;
            }
            case 2: {   // Shield
                if (!heavy_hero->HasBuff("Shield")) {
                    heavy_hero = new ShieldDecorator(heavy_hero);
                    std::cout << "[Buff] Append Shield to HeavyHero" << std::endl;
                } else {
                    std::cout << "[Buff] There is Shield already with HeavyHero" << std::endl;
                }
                break;
            }
            case 3: {   // Helmet
                if (!heavy_hero->HasBuff("Helmet")) {
                    heavy_hero = new HelmetDecorator(heavy_hero);
                    std::cout << "[Buff] Append Helmet to HeavyHero" << std::endl;
                } else {
                    std::cout << "[Buff] There is Helmet already with HeavyHero" << std::endl;
                }
                break;
            }
            default: {
                std::cout << "[Buff] Unknown buff for HeavyHero" << std::endl;
                break;
            }
        }
    }

    return heavy_hero;
}