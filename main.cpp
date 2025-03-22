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
    srand(time(nullptr));

    HeavyHero heavy_hero{};
    Hero hero{};

    Wizard wizard{};
    HeavyHero* heavy_hero_clone = dynamic_cast<HeavyHero*>(wizard.PerformSpecAction(&heavy_hero));
    Hero* hero_clone = dynamic_cast<Hero*>(wizard.PerformSpecAction(&hero));

    heavy_hero.PerformAttack(hero_clone);

    Hiller hiller{};
    hiller.PerformSpecAction(&hiller);
    hiller.PerformSpecAction(hero_clone);

    return 0;
}