#include "team/UnitNumberManager.h"

unsigned int UnitNumberManager::AssignNumber(IUnit* hero) {
    unsigned int number = next_number_++;
    hero_numbers_[hero] = number;
    return number;
}

unsigned int UnitNumberManager::GetNumber(IUnit* hero) const {
    if (
        dynamic_cast<HeavyHero*>(hero) &&
        ExtractHeavyHeroTypeFromUnitPtr(dynamic_cast<HeavyHero*>(hero)) != "HeavyHero"
        ) {
        hero = dynamic_cast<HeavyHeroDecorator*>(hero)->GetInnerHeavyHeroOrigin();
        }
    auto it = hero_numbers_.find(hero);
    if (it != hero_numbers_.end()) {
        return it->second;
    }
    return 0;
}

void UnitNumberManager::ReplaceHero(IUnit* prev_hero, IUnit* new_hero) {
    if (
        dynamic_cast<HeavyHero*>(prev_hero) &&
        ExtractHeavyHeroTypeFromUnitPtr(dynamic_cast<HeavyHero*>(prev_hero)) == "HeavyHero"
        ) {
        return;
        }
    unsigned int prev_number = hero_numbers_[prev_hero];
    hero_numbers_.erase(prev_hero);
    hero_numbers_[new_hero] = prev_number;
}