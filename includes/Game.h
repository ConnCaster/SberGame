#ifndef SBERGAME_GAME_H
#define SBERGAME_GAME_H

#include <iostream>

#include "team/TeamBuilder.h"

// @brief Pattern Singlton and LazyInitialization
class Game {
public:
    static Game* GetInstance();
    ~Game();

    void Run();

private:
    Game();

    int SetTeamGenerationType();
    Team* CreateTeam(ITeamBuilder* team_builder);

    void ShowGameResults() const;
private:
    static Game* game_instance_;

    Team* red_;
    Team* blue_;
    ITeamBuilder* red_team_builder_;
    ITeamBuilder* blue_team_builder_;
};

static bool IsBuildTypeCorrect(unsigned int type) {
    return (type <= 3 && type > 0);
}

void Fight(IUnit* l, Team* l_team, IUnit* r, Team* r_team);

#endif //SBERGAME_GAME_H
