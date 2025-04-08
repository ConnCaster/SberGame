#include "Game.h"

int main() {
    srand(time(nullptr));

//    HeavyHero* heavy_hero = new HeavyHero{};
//    Hero* hero = new Hero{};

    /*
    heavy_hero = new HorseDecorator(heavy_hero);
    heavy_hero = new ShieldDecorator(heavy_hero);
    heavy_hero = new SpearDecorator(heavy_hero);
    hero->PerformAttack(heavy_hero);
    heavy_hero->PerformAttack(hero);
    */

    Game* game = Game::GetInstance();
    game->Run();

    delete game;
    return 0;
}