#include "Game.h"

int main() {
    srand(time(nullptr));

    HeavyHero* heavy_hero = new HeavyHero{};
    Hero* hero = new Hero{};

    heavy_hero = AppendBuffToHeavyHero(heavy_hero);
    hero->PerformAttack(heavy_hero);
    std::cout << "==========================" << std::endl;
    hero->PerformAttack(heavy_hero);

//    Game* game = Game::GetInstance();
//    game->Run();
//    delete game;
    return 0;
}