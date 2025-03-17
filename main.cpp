#include <iostream>

#include "interfaces/IAttack.h"
#include "interfaces/ISpecAction.h"
#include "interfaces/IUnit.h"

#include "heros/HeavyHero.h"
#include "heros/Hero.h"
#include "heros/Archer.h"
#include "heros/Hiller.h"

#include "attacks/NormalAttack.h"
#include "spec_actions/HillSpecAction.h"

int main() {
    HeavyHero heavy_hero{};
    heavy_hero.SetAttack(std::make_unique<NormalAttack>());

    Hero hero{};
    hero.SetAttack(std::make_unique<NormalAttack>());

    hero.PerformAttack(&heavy_hero);


    return 0;
}