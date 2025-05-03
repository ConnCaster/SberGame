#include <iostream>

#include "Game.h"
#include "Attack.h"

Game::Game()
    : red_{nullptr},
      blue_{nullptr},
      red_team_builder_{nullptr},
      blue_team_builder_{nullptr},
      logger_{},
      current_state_{nullptr} {
}

Game::~Game() {
    delete red_;
    delete blue_;
    delete red_team_builder_;
    delete blue_team_builder_;
}

void ChooseFirstTurnTeam(std::string &first_team) {
    do {
        std::cout << "Enter first command [red/blue]: ";
        std::cin >> first_team;
        if (first_team != "red" && first_team != "blue") {
            std::cout << "Unknown command. Try again ... \n";
        }
    } while (first_team != "red" && first_team != "blue");
}

GameMode ChooseGameMode() {
    std::string game_mode{0};
    do {
        std::cout << "Enter mode you want to play [1-StepByStep / 2-FastRun] ";
        std::cin >> game_mode;
        if (game_mode != "1" && game_mode != "2") {
            std::cout << "Unknown command. Try again ... \n";
        }
    } while (game_mode != "1" && game_mode != "2");
    return (game_mode == "1") ? GameMode::StepByStep : GameMode::FastRun;
}


void Game::Run() {
    SetTeamGenerationType();
    std::cout << "\n==== Start game ====\n";
    red_ = CreateTeam(red_team_builder_);
    blue_ = CreateTeam(blue_team_builder_);

    std::string first_team{};
    ChooseFirstTurnTeam(first_team);
    red_team_order_ = (first_team == "red");

    current_state_ = (ChooseGameMode() == GameMode::StepByStep) ? IGameState::CreateWaitingState() : nullptr;
    while (red_ && blue_ && !red_->IsEmpty() && !blue_->IsEmpty()) {
        std::cout << "=====================================" << std::endl;
        if (current_state_) {
            current_state_->Update(*this);

            std::cout << "Enter 'n': ";
            std::string input;
            std::cin >> input;
            current_state_->HandleInput(*this, input);
        } else {
            ProcessTurnLogic();
        }
    }
    ShowGameResults();
}

void Game::Turn() {
    IUnit *red_unit = red_->GetUnit();
    IUnit *blue_unit = blue_->GetUnit();
    AttackFacade attack_facade_red{red_, blue_, &logger_};
    AttackFacade attack_facade_blue{blue_, red_, &logger_};
    if (red_team_order_) {
        // герой красных наносит удар
        int is_killed_blue = attack_facade_red.Attack(red_unit, blue_unit);
        int is_killed_red{0};
        if (!is_killed_blue) {
            // если синий противник выжил, то он отвечает
            red_unit = red_->GetUnit();
            blue_unit = blue_->GetUnit();
            is_killed_red = attack_facade_blue.Attack(blue_unit, red_unit);
        }
        // применяются специальные действия
        SpecAction(red_, blue_, is_killed_red);
        SpecAction(blue_, red_, is_killed_blue);
        red_team_order_ = false;
    } else {
        // герой синих наносит удар
        int is_killed_red = attack_facade_blue.Attack(blue_unit, red_unit);
        int is_killed_blue{0};
        if (!is_killed_red) {
            // если красный противник выжил, то он отвечает
            red_unit = red_->GetUnit();
            blue_unit = blue_->GetUnit();
            is_killed_blue = attack_facade_red.Attack(red_unit, blue_unit);
        }
        // применяются специальные действия
        SpecAction(blue_, red_, is_killed_blue);
        SpecAction(red_, blue_, is_killed_red);
        red_team_order_ = true;
    }
}

unsigned int Game::ChooseTeamBuilderType() {
    std::string team_builder_type{};

    std::cout << "Enter team builder type [Random/Greedy/ByHands]: ";
    while (true) {
        std::cin >> team_builder_type;
        if (team_builder_type == "Random") {
            return 1;
        } else if (team_builder_type == "Greedy") {
            return 2;
        } else if (team_builder_type == "ByHands") {
            return 3;
        } else {
            std::cout << "Unknown team builder type. Choose again [Random/Greedy/ByHands]: ";
        }
    }
}

int Game::SetTeamGenerationType() {
    delete red_team_builder_;
    delete blue_team_builder_;

    unsigned int type = ChooseTeamBuilderType();
    std::cout << "Enter team maximum cost: ";
    unsigned int team_max_cost{0};
    std::cin >> team_max_cost;

    red_team_builder_ = ITeamBuilderFactory::CreateTeamBuilder(type, "Red", team_max_cost);
    blue_team_builder_ = ITeamBuilderFactory::CreateTeamBuilder(type, "Blue", team_max_cost);
    return 0;
}

Team *Game::CreateTeam(ITeamBuilder *team_builder) {
    Director director{team_builder};
    director.ConstructTeam();
    return team_builder->GetTeam();
}

void Game::ShowGameResults() const {
    std::cout << "\n==== STATISTICS =====\n";
    if (red_ && red_->IsEmpty()) {
        std::cout << "Blue team WIN!" << std::endl;
        std::cout << "Past units: " << blue_->GetSize() << std::endl;
        std::cout << blue_->GetTeamInfo() << std::endl;
        TeamIterator *iter = blue_->CreateIterator();
        int i = 0;
        std::cout << "=======================" << std::endl;
        while (iter->HasNext()) {
            IUnit *unit = iter->Next();
            std::cout << "\t[" + std::to_string(i + 1) + "] " + ExtractTypeFromUnitPtr(unit) + " [index=" +
                    std::to_string(blue_->GetHeroNumber(unit)) + "]\n\t\t" + unit->GetInfo() + "\n" << std::endl;
            i++;
        }
        delete iter;
    } else if (blue_ && blue_->IsEmpty()) {
        std::cout << "Red team WIN!" << std::endl;
        std::cout << "Past units: " << red_->GetSize() << std::endl;
        std::cout << red_->GetTeamInfo() << std::endl;
        TeamIterator *iter = red_->CreateIterator();
        int i = 0;
        std::cout << "=======================" << std::endl;
        while (iter->HasNext()) {
            IUnit *unit = iter->Next();
            std::cout << "\t[" + std::to_string(i + 1) + "] " + ExtractTypeFromUnitPtr(unit) + " [index=" +
                    std::to_string(red_->GetHeroNumber(unit)) + "]\n\t\t" + unit->GetInfo() << std::endl;
            i++;
        }
        delete iter;
    }
}

// "[index=" << l_team_->GetHeroNumber(l) << "]"
void Game::SpecAction(Team *l_team, Team *r_team, int was_killed) {
    std::string msg = "\n[" + l_team->GetTeamName() + "] use SpecActions via [" + r_team->GetTeamName() + "]\n";
    std::cout << msg;
    logger_.AddLogMsg(msg, LogType::SPEC_ACT);
    for (int i = (was_killed) ? 0 : 1; i < l_team->GetSize(); ++i) {
        IUnit *unit = l_team->GetUnitByPos(i);
        // TODO: как применять специальные действия
        if (ExtractTypeFromUnitPtr(unit) == "HeavyHero") {
            msg = "[" + ExtractTypeFromUnitPtr(unit) + "] [" + std::to_string(l_team->GetHeroNumber(unit)) +
                  "] Does not have special ability\n";
            std::cout << msg;
            logger_.AddLogMsg(msg, LogType::SPEC_ACT);
        } else if (ExtractTypeFromUnitPtr(unit) == "Hero") {
            msg = "[" + ExtractTypeFromUnitPtr(unit) + "] [index=" + std::to_string(l_team->GetHeroNumber(unit)) +
                  "] Try to append buff to neighbour HeavyHero if he exists\n";


            // Добавляем усилители HeavyHero
            std::vector<std::pair<IUnit *, unsigned int> > heavy_heros_vec = l_team->CheckIfHeavyHeroNeighbour(
                l_team->GetSize() - 1);
            for (auto &[heavy_hero, pos]: heavy_heros_vec) {
                if (heavy_hero) {
                    HeavyHero *heavy_hero_with_buff = AppendBuffToHeavyHero(dynamic_cast<HeavyHero *>(heavy_hero));
                    // заменяем героя в его позиция на героя с бафами
                    l_team->ReplaceUnit(heavy_hero_with_buff, pos);
                    msg += "Buff [" + ExtractHeavyHeroTypeFromUnitPtr(heavy_hero_with_buff) +
                            "] was added successfully to [HeavyHero] [index=" + std::to_string(
                                l_team->GetHeroNumber(heavy_hero_with_buff)) + "]\n";
                } else {
                }
            }
            std::cout << msg;
            logger_.AddLogMsg(msg, LogType::SPEC_ACT);
        } else if (ExtractTypeFromUnitPtr(unit) == "Archer") {
            msg = "[" + ExtractTypeFromUnitPtr(unit) + "] [index=" + std::to_string(l_team->GetHeroNumber(unit)) +
                  "] Does not have special ability\n";
            std::cout << msg;
            logger_.AddLogMsg(msg, LogType::SPEC_ACT);
        } else if (ExtractTypeFromUnitPtr(unit) == "Hiller") {
            msg = "[" + ExtractTypeFromUnitPtr(unit) + "] [index=" + std::to_string(l_team->GetHeroNumber(unit)) +
                  "] Try to hill unit from his team\n";
            std::cout << msg;
            logger_.AddLogMsg(msg, LogType::SPEC_ACT);
            // Лечим героя из своей команды (легкого, лучника)
            unsigned int pos = l_team->GenPosAroundUnit(i, dynamic_cast<Hiller *>(unit)->GetHillDistance());
            IUnit *unit_to_hill = l_team->GetUnitByPos(pos);
            if (dynamic_cast<ICanBeHilled *>(unit_to_hill) == nullptr) {
                // если лечить нельзя
                msg = "[" + ExtractTypeFromUnitPtr(unit_to_hill) + "] [index=" + std::to_string(
                          l_team->GetHeroNumber(unit_to_hill)) + "] Does not have ability to be hilled\n";
                std::cout << msg;
                logger_.AddLogMsg(msg, LogType::SPEC_ACT);
            } else {
                msg = "Hiller  [index=" + std::to_string(l_team->GetHeroNumber(unit)) + "] hills... " +
                      ExtractTypeFromUnitPtr(unit_to_hill) + " [index=" + std::to_string(
                          l_team->GetHeroNumber(unit_to_hill)) + "]\n";
                std::cout << msg;
                logger_.AddLogMsg(msg, LogType::SPEC_ACT);
                dynamic_cast<Hiller *>(unit)->PerformSpecAction(unit_to_hill);
            }
        } else if (ExtractTypeFromUnitPtr(unit) == "Wizard") {
            msg = "[" + ExtractTypeFromUnitPtr(unit) + "] [index=" + std::to_string(l_team->GetHeroNumber(unit)) +
                  "] Try to clone unit from his team\n";
            std::cout << msg;
            logger_.AddLogMsg(msg, LogType::SPEC_ACT);
            // Клонирование героя из своей команды (легкого, лучника, хиллера)
            int pos = rand() % l_team->GetSize();
            IUnit *unit_to_clone = l_team->GetUnitByPos(pos);
            if (dynamic_cast<ICanBeCloned *>(unit_to_clone) == nullptr) {
                // если клонировать нельзя
                msg = "[" + ExtractTypeFromUnitPtr(unit_to_clone) + "] [index=" + std::to_string(
                          l_team->GetHeroNumber(unit_to_clone)) + "] Does not have ability to be clonned\n";
                std::cout << msg;
                logger_.AddLogMsg(msg, LogType::SPEC_ACT);
            } else {
                msg = "Wizard [index=" + std::to_string(l_team->GetHeroNumber(unit)) + "] clone... " +
                      ExtractTypeFromUnitPtr(unit_to_clone) + "\n";
                std::cout << msg;
                logger_.AddLogMsg(msg, LogType::SPEC_ACT);
                IUnit *cloned_unit = dynamic_cast<Wizard *>(unit)->PerformSpecAction(unit_to_clone);
                if (cloned_unit) {
                    l_team->AddUnitToPos(cloned_unit, i++);
                }
            }
        } else if (ExtractTypeFromUnitPtr(unit) == "Wagenburg") {
            msg = "[" + ExtractTypeFromUnitPtr(unit) + "] [index=" + std::to_string(l_team->GetHeroNumber(unit)) +
                  "] Does not have special ability\n";
            std::cout << msg;
            logger_.AddLogMsg(msg, LogType::SPEC_ACT);
        }
    }
}

// @brief Pattern Fabric Method
std::unique_ptr<IGameState> IGameState::CreateWaitingState() {
    return std::make_unique<WaitingForInputState>();
}

std::unique_ptr<IGameState> IGameState::CreateProcessingState() {
    return std::make_unique<ProcessingTurnState>();
}

// Работа с состояниями в самой игре
void Game::SetState(std::unique_ptr<IGameState> new_state) {
    current_state_ = std::move(new_state);
}

// void Game::HandleInput(std::string& input) {
//     if (current_state_) {
//         current_state_->HandleInput(*this, input);
//     }
// }

// Обработка состояния ожидания хода
void WaitingForInputState::Update(Game &game) {
    // Ничего не делаем, ждем ввода
}

void WaitingForInputState::HandleInput(Game &game, std::string &input) {
    if (input == "n") {
        game.SetState(IGameState::CreateProcessingState());
    } else {
        std::cout << "Error. Enter 'n' again: ";
        std::cin >> input;
        HandleInput(game, input);
        // game.HandleInput(input);
    }
}

// Обработка состояния хода
void ProcessingTurnState::Update(Game &game) {
    game.ProcessTurnLogic();
    game.SetState(IGameState::CreateWaitingState());
}

void ProcessingTurnState::HandleInput(Game &game, std::string &input) {
    // Игнорируем ввод
}
