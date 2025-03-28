#include "Game.h"

int main() {
    srand(time(nullptr));

    Game* game = Game::GetInstance();
    game->Run();

    delete game;
    return 0;
}