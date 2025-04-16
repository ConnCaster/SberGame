#ifndef SBERGAME_GAME_H
#define SBERGAME_GAME_H

#include "team/TeamBuilder.h"
#include "logger/Logger.h"

enum class GameMode: int {
    StepByStep = 1,
    FastRun = 2
};

class Game;
class IGameState {
public:
    virtual ~IGameState() = default;
    virtual void Update(Game& game) = 0;
    virtual void HandleInput(Game& game, std::string& input) = 0;

    // @brief Pattern Fabric Method
    // для состояний
    static std::unique_ptr<IGameState> CreateWaitingState();
    static std::unique_ptr<IGameState> CreateProcessingState();
};

// @brief Pattern LazyInitialization
class Game {
public:
    Game();
    ~Game();

    void Run();

// ================================
    void SetState(std::unique_ptr<IGameState> new_state);
    void ProcessTurnLogic() { Turn(); };
    // void HandleInput(std::string& input);

private:
    int SetTeamGenerationType();
    Team* CreateTeam(ITeamBuilder* team_builder);

    int Attack(IUnit* l, Team* l_team, IUnit* r, Team* r_team);
    void SpecAction(Team* l_team, Team* r_team, int was_killed);

    void ShowGameResults() const;

    void Turn();

private:
    Team* red_;
    Team* blue_;
    ITeamBuilder* red_team_builder_;
    ITeamBuilder* blue_team_builder_;

    Logger logger_death_;
    Logger logger_spec_acts_;

    bool red_team_order_ = true;

    std::unique_ptr<IGameState> current_state_;
};

void ChooseFirstTurnTeam(std::string& first_team);
GameMode ChooseGameMode();

// =======================================
// Реализация WaitingState
class WaitingForInputState : public IGameState {
public:
    void Update(Game& game) override;
    void HandleInput(Game& game, std::string& input) override;
};


// Реализация ProcessingState
class ProcessingTurnState : public IGameState {
public:
    void Update(Game& game) override;
    void HandleInput(Game& game, std::string& input) override;
};

#endif //SBERGAME_GAME_H
