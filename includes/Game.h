#ifndef SBERGAME_GAME_H
#define SBERGAME_GAME_H

#include <iostream>

#include "team/TeamBuilder.h"
#include "logger/Logger.h"

// @brief Pattern LazyInitialization
class Game {
public:
    Game();
    ~Game();

    void Run();

private:
    int SetTeamGenerationType();
    Team* CreateTeam(ITeamBuilder* team_builder);

    int Attack(IUnit* l, Team* l_team, IUnit* r, Team* r_team);
    void SpecAction(Team* l_team, Team* r_team, int was_killed);

    void ShowGameResults() const;


private:
    Team* red_;
    Team* blue_;
    ITeamBuilder* red_team_builder_;
    ITeamBuilder* blue_team_builder_;

    Logger logger_death_;
    Logger logger_spec_acts_;
};


#endif //SBERGAME_GAME_H
