#ifndef SBERGAME_GAME_H
#define SBERGAME_GAME_H

#include <iostream>

#include "team/TeamBuilder.h"
#include "logger/Logger.h"

class Game;

class GameState {
public:
    virtual ~GameState() = default;
    virtual void Enter(Game& game) = 0;
    virtual void Update(Game& game) = 0;
    virtual void HandleInput(Game& game, char input) = 0;

    // Фабричные методы для состояний
    static std::unique_ptr<GameState> CreateWaitingState();
    static std::unique_ptr<GameState> CreateProcessingState();
};

// @brief Pattern LazyInitialization
class Game {
public:
    Game();
    ~Game();

    void Run();

// ================================
    void ChangeState(std::unique_ptr<GameState> new_state);
    void ProcessTurnLogic() {
        NextTurn();
    };  // Переносим сюда логику из ProcessNextTurn
    void HandleInput(char input);

private:
    int SetTeamGenerationType();
    Team* CreateTeam(ITeamBuilder* team_builder);

    int Attack(IUnit* l, Team* l_team, IUnit* r, Team* r_team);
    void SpecAction(Team* l_team, Team* r_team, int was_killed);

    void ShowGameResults() const;

    void NextTurn();

private:
    Team* red_;
    Team* blue_;
    ITeamBuilder* red_team_builder_;
    ITeamBuilder* blue_team_builder_;

    Logger logger_death_;
    Logger logger_spec_acts_;

    bool red_team_order_ = true;

    std::unique_ptr<GameState> current_state_;
};


// =======================================
// Реализация WaitingState
class WaitingForInputState : public GameState {
public:
    void Enter(Game& game) override;
    void Update(Game& game) override;
    void HandleInput(Game& game, char input) override;
};


// Реализация ProcessingState
class ProcessingTurnState : public GameState {
public:
    void Enter(Game& game) override;
    void Update(Game& game) override;
    void HandleInput(Game& game, char input) override;
};

#endif //SBERGAME_GAME_H
