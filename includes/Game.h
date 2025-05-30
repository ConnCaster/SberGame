#ifndef SBERGAME_GAME_H
#define SBERGAME_GAME_H

#include <stack>

#include "team/TeamBuilder.h"
#include "logger/Logger.h"
#include "Command.h"

enum class GameMode: int {
    StepByStep = 1,
    FastRun = 2
};

enum class GameRuntimeState: int {
    FIRST_RUN = 1,
    RESTARTED = 2,
    IN_PROGRESS = 3,
    FINISH = 4
};

class IGameState;

class IGame {
public:
    virtual ~IGame() = default;

    virtual void ProcessTurnLogic() = 0;
    virtual bool UndoLastTurn() = 0;

    virtual void Turn() = 0;

    virtual Team* GetRedTeam() = 0;
    virtual Team* GetBlueTeam() = 0;
    virtual bool GetRedTeamOrder() = 0;
    virtual void SetRedTeam(Team* team) = 0;
    virtual void SetBlueTeam(Team* team) = 0;
    virtual void SetRedTeamOrder(bool order) = 0;

    virtual void RestoreInitialState() = 0;
    virtual void SaveInitialState() = 0;

    virtual CommandManager& GetCommandManager() = 0;

    virtual void SetState(std::unique_ptr<IGameState> new_state) = 0;
};

// Конкретная команда для выполнения хода
class TurnCommand : public ICommand {
public:
    explicit TurnCommand(IGame& game) : game_(game) {}

    void Execute() override {
        // Сохраняем состояние перед выполнением хода
        SaveState();
        game_.Turn();
    }

    void Undo() override {
        // Восстанавливаем сохраненное состояние
        RestoreState();
    }

private:
    void SaveState() {
        saved_state_.red_team_copy = new Team(*game_.GetRedTeam());
        saved_state_.blue_team_copy = new Team(*game_.GetBlueTeam());
        saved_state_.red_team_order_copy = game_.GetRedTeamOrder();
    }
    void RestoreState() {
        game_.SetRedTeam(saved_state_.red_team_copy);
        game_.SetBlueTeam(saved_state_.blue_team_copy);
        game_.SetRedTeamOrder(saved_state_.red_team_order_copy);
    }

    IGame& game_;
    // Здесь нужно сохранить все необходимые данные для отмены хода
    struct GameState {
        // Добавьте сюда все поля, которые нужно сохранять для отмены хода
        Team* red_team_copy;
        Team* blue_team_copy;
        bool red_team_order_copy;
        // и т.д.
    };
    GameState saved_state_;
};

class ResetToInitialCommand : public ICommand {
public:
    explicit ResetToInitialCommand(IGame& game) : game_(game) {}

    void Execute() override {
        // Сохраняем текущее состояние перед сбросом (для возможного Undo)
        SaveState();
        game_.RestoreInitialState();
    }

    void Undo() override {
        // Восстанавливаем состояние перед сбросом
        RestoreState();
    }

private:
    void SaveState() {
        saved_state_.red_team_copy = new Team(*game_.GetRedTeam());
        saved_state_.blue_team_copy = new Team(*game_.GetBlueTeam());
        saved_state_.red_team_order_copy = game_.GetRedTeamOrder();
    }

    void RestoreState() {
        game_.SetRedTeam(saved_state_.red_team_copy);
        game_.SetBlueTeam(saved_state_.blue_team_copy);
        game_.SetRedTeamOrder(saved_state_.red_team_order_copy);
    }

    IGame& game_;
    struct GameState {
        Team* red_team_copy;
        Team* blue_team_copy;
        bool red_team_order_copy;
    };
    GameState saved_state_;
};

class Game;

class IGameState {
public:
    virtual ~IGameState() = default;
    virtual void Update(Game& game) = 0;
    virtual void HandleInput(Game& game, std::string& input) = 0;

    // @brief Pattern Fabric Method
    static std::unique_ptr<IGameState> CreateWaitingState();
    static std::unique_ptr<IGameState> CreateProcessingState();
};

// @brief Pattern LazyInitialization
class Game : public IGame {
public:
    Game();
    ~Game() override;

    void Run();

// ================================
    void SetState(std::unique_ptr<IGameState> new_state);
    void ProcessTurnLogic() override {
        command_manager_.ExecuteCommand(new TurnCommand(*this));
        // Turn();
    };

    bool UndoLastTurn() {
        return command_manager_.UndoLastCommand();
    }

    Team* GetRedTeam() override { return red_; }
    Team* GetBlueTeam() override { return blue_; }
    bool GetRedTeamOrder() override { return red_team_order_; }
    void SetRedTeam(Team* team) override {
        delete red_;
        red_ = team;
    }
    void SetBlueTeam(Team* team) override {
        delete blue_;
        blue_ = team;
    }
    void SetRedTeamOrder(bool order) override { red_team_order_ = order; }
    // void HandleInput(std::string& input);

    void SaveInitialState() {
        initial_state_.initial_red_team = new Team(*red_);
        initial_state_.initial_blue_team = new Team(*blue_);
        initial_state_.initial_red_team_order = red_team_order_;
    }

    void RestoreInitialState() {
        SetRedTeam(new Team(*initial_state_.initial_red_team));
        SetBlueTeam(new Team(*initial_state_.initial_blue_team));
        SetRedTeamOrder(initial_state_.initial_red_team_order);
    }

    CommandManager& GetCommandManager() { return command_manager_; }

private:
    int SetTeamGenerationType();
    unsigned int ChooseTeamBuilderType();
    Team* CreateTeam(ITeamBuilder* team_builder);


    void SpecAction(Team* l_team, Team* r_team, int was_killed, int start_pos=0);

    void ShowGameResults() const;

    void Turn();

private:
    Team* red_;
    Team* blue_;
    ITeamBuilder* red_team_builder_;
    ITeamBuilder* blue_team_builder_;
    std::unique_ptr<IGameState> current_state_;

    LogMsgHandler logger_{};

    bool red_team_order_ = true;
    GameRuntimeState start_game_from_beginning = GameRuntimeState::FIRST_RUN;

    CommandManager command_manager_{};
    struct InitialGameState {
        Team* initial_red_team;
        Team* initial_blue_team;
        bool initial_red_team_order;
    };
    InitialGameState initial_state_{};


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
