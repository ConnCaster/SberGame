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

#include "Game.h"

int main() {
    srand(time(nullptr));

    Game* game = Game::GetInstance();
    game->Run();

    delete game;
    return 0;
}