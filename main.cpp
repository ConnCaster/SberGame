#include <iostream>

#include "interfaces/IAttack.h"
#include "interfaces/ISpecAction.h"
#include "interfaces/IUnit.h"

#include "heros/HeavyHero.h"
#include "heros/Hero.h"
#include "heros/Archer.h"
#include "heros/Hiller.h"
#include "heros/Wizard.h"

#include "attacks/NormalAttack.h"
#include "spec_actions/HillSpecAction.h"

int main() {
    HeavyHero heavy_hero{};
    Hero hero{};

    Wizard wizard{};
    IUnit* clone1 = wizard.PerformSpecAction(&heavy_hero);
    IUnit* clone2 = wizard.PerformSpecAction(&hero);

//    Hero hero{};
//    hero.SetAttack(std::make_unique<NormalAttack>());

//    hero.PerformAttack(&heavy_hero);


    return 0;
}