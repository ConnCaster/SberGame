#ifndef SBERGAME_GAME_H
#define SBERGAME_GAME_H

#include "Team.h"

// @brief Pattern Singlton
class Game {
public:
    static Game* GetInstance();
    ~Game();

    void Run();

private:
    Game();

private:
    static Game* game_instance_;

    Team* red_;
    Team* blue_;
};

#endif //SBERGAME_GAME_H
