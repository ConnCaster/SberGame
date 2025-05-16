#include <iostream>

#include "Game.h"
#include "Attack.h"
#include "team/TeamReport.h"

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
    if (!red_) {
        std::cout << "Error while creating team. Abort..." << std::endl;
        return;
    }
    blue_ = CreateTeam(blue_team_builder_);
    if (!blue_) {
        std::cout << "Error while creating team. Abort..." << std::endl;
        return;
    }


    // Добавляем выбор построения
    std::string formation;
    std::cout << "Choose formation for teams [line_first/line_all/column]: ";
    std::cin >> formation;

    FormationType formation_type;
    if (formation == "line_first") {
        formation_type = FormationType::LINE_FIRST_ONLY;
    } else if (formation == "line_all") {
        formation_type = FormationType::LINE_ALL_ATTACK;
    } else if (formation == "column") {
        formation_type = FormationType::COLUMN;
    } else {
        std::cout << "Unknown formation, using default (line_first)\n";
        formation_type = FormationType::LINE_FIRST_ONLY;
    }

    // Устанавливаем построение
    if (red_) red_->SetFormation(formation_type);
    if (blue_) blue_->SetFormation(formation_type);

    std::string first_team{};
    ChooseFirstTurnTeam(first_team);
    red_team_order_ = (first_team == "red");

    current_state_ = (ChooseGameMode() == GameMode::StepByStep) ? IGameState::CreateWaitingState() : nullptr;
    while (red_ && blue_ && !red_->IsEmpty() && !blue_->IsEmpty()) {
        std::cout << "=====================================" << std::endl;
        if (current_state_) {
            current_state_->Update(*this);

            std::cout << "Enter 'n' (next turn) or 'u' (undo turn): ";
            std::string input;
            std::cin >> input;
            if (input == "u") {
                if (!UndoLastTurn()) {
                    std::cout << "Nothing to undo!" << std::endl;
                }
            } else {
                current_state_->HandleInput(*this, input);
            }
        } else {
            ProcessTurnLogic();
        }
    }
    ShowGameResults();
}

void Game::Turn() {
    // IUnit *red_unit = red_->GetUnit();
    // IUnit *blue_unit = blue_->GetUnit();
    AttackMediator* attack_mediator_red_to_blue = new UnitToUnitAttackMediator(red_, blue_, &logger_);
    AttackMediator* attack_mediator_blue_to_red = new UnitToUnitAttackMediator(blue_, red_, &logger_);

    AttackFacade attack_facade_red{attack_mediator_red_to_blue};
    AttackFacade attack_facade_blue{attack_mediator_blue_to_red};

    if (red_team_order_) {
        // герой красных наносит удар
        int is_killed_blue = 0;
        if (red_->GetFormation() == FormationType::LINE_FIRST_ONLY) {
            // auto [red_attacker, pos] = red_->GetNextAttacker();
            IUnit* red_attacker = red_->GetUnit();
            if (red_attacker) {
                IUnit* blue_target = blue_->GetUnit();
                is_killed_blue = attack_facade_red.Attack(red_attacker, blue_target);
                if (!is_killed_blue && blue_target->IsAlive()) {
                    // Контратака синей команды
                    // auto [blue_attacker, pos] = blue_->GetNextAttacker();
                    IUnit* blue_attacker = blue_->GetUnit();
                    if (blue_attacker) {
                        IUnit* red_target = red_->GetUnit();
                        attack_facade_blue.Attack(blue_attacker, red_target);
                    }
                }
            }
            SpecAction(red_, blue_, 0);
            SpecAction(blue_, red_, is_killed_blue);
        } else if (red_->GetFormation() == FormationType::LINE_ALL_ATTACK) {
            int blue_i = 0, red_j = 0;
            while (blue_i < blue_->GetSize() && red_j < red_->GetSize()) {
                std::cout << "[BLUE_OPPONENT] SIZE: " << blue_->GetSize() << std::endl;
                IUnit* red_attacker = red_->GetUnitByPosAndRemove(red_j);
                if (red_attacker) {
                    IUnit* blue_target = blue_->GetUnitByPosAndRemove(blue_i);
                    is_killed_blue = attack_facade_red.Attack(red_attacker, blue_target);
                    red_->ReturnUnitToPos(red_attacker, red_j);
                    if (!is_killed_blue && blue_target->IsAlive()) {
                        // Контратака синей команды
                        blue_->ReturnUnitToPos(blue_target, blue_i);
                        IUnit* blue_attacker = blue_->GetUnitByPosAndRemove(blue_i);
                        if (blue_attacker) {
                            IUnit* red_target = red_->GetUnitByPosAndRemove(red_j);
                            bool is_killed_red_local = attack_facade_blue.Attack(blue_attacker, red_target);
                            blue_->ReturnUnitToPos(blue_attacker, blue_i);
                            if (!is_killed_red_local && red_target->IsAlive()) {
                                red_->ReturnUnitToPos(red_target, red_j);
                            } else {
                                red_j++;
                            }
                        }
                    } else {
                        blue_i++;
                    }
                }
                red_j++;
                blue_i++;
            }
            SpecAction(red_, blue_, 1, red_j);
            SpecAction(blue_, red_, 1, blue_i);
        }
        red_team_order_ = false;
    } else {
        // Атака синей команды
        int is_killed_red = 0;
        if (blue_->GetFormation() == FormationType::LINE_FIRST_ONLY) {
            // auto [blue_attacker, pos] = blue_->GetNextAttacker();
            IUnit* blue_attacker = blue_->GetUnit();
            if (blue_attacker) {
                IUnit* red_target = red_->GetUnit();
                is_killed_red = attack_facade_blue.Attack(blue_attacker, red_target);
                int is_killed_blue{0};
                if (!is_killed_red && red_target->IsAlive()) {
                    // Контратака красной команды
                    // auto [red_attacker, pos] = red_->GetNextAttacker();
                    IUnit* red_attacker = red_->GetUnit();
                    if (red_attacker) {
                        IUnit* blue_target = blue_->GetUnit();
                        attack_facade_red.Attack(red_attacker, blue_target);
                    }
                }
            }
            // Специальные действия
            SpecAction(blue_, red_, 0);
            SpecAction(red_, blue_, is_killed_red);
        } else if (blue_->GetFormation() == FormationType::LINE_ALL_ATTACK) {
            int blue_i = 0, red_j = 0;
            while (blue_i < blue_->GetSize() && red_j < red_->GetSize()) {
                std::cout << "[RED_OPPONENT] SIZE: " << red_->GetSize() << std::endl;
                IUnit* blue_attacker = blue_->GetUnitByPosAndRemove(blue_i);
                if (blue_attacker) {
                    IUnit* red_target = red_->GetUnitByPosAndRemove(red_j);
                    is_killed_red = attack_facade_blue.Attack(blue_attacker, red_target);
                    blue_->ReturnUnitToPos(blue_attacker, blue_i);
                    if (!is_killed_red && red_target->IsAlive()) {
                        // Контратака красной команды
                        red_->ReturnUnitToPos(red_target, red_j);
                        IUnit* red_attacker = red_->GetUnitByPosAndRemove(red_j);
                        if (red_attacker) {
                            IUnit* blue_target = blue_->GetUnitByPosAndRemove(blue_i);
                            bool is_killed_blue_local = attack_facade_red.Attack(red_attacker, blue_target);
                            red_->ReturnUnitToPos(red_attacker, red_j);
                            if (!is_killed_blue_local && blue_target->IsAlive()) {
                                blue_->ReturnUnitToPos(blue_target, blue_i);
                            } else {
                                blue_i++;
                            }
                        }
                    } else {
                        red_j++;
                    }
                }
                red_j++;
                blue_i++;
            }
            // Специальные действия
            SpecAction(blue_, red_, 1, blue_i);
            SpecAction(red_, blue_, 1, red_j);
        }
        red_team_order_ = true;
    }
}

unsigned int Game::ChooseTeamBuilderType() {
    std::string team_builder_type{};

    std::cout << "Enter team builder type [Random/Greedy/ByHands/AllHeros/FromFile]: ";
    while (true) {
        std::cin >> team_builder_type;
        if (team_builder_type == "Random") {
            return 1;
        } else if (team_builder_type == "Greedy") {
            return 2;
        } else if (team_builder_type == "ByHands") {
            return 3;
        } else if (team_builder_type == "AllHeros") {
            return 4;
        } else if (team_builder_type == "FromFile") {
            return 5;
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

        std::unique_ptr<ReportGenerator> team_report = std::make_unique<TeamReportGenerator>(blue_);
        team_report->GenerateReport();
    } else if (blue_ && blue_->IsEmpty()) {
        std::cout << "Red team WIN!" << std::endl;
        std::cout << "Past units: " << red_->GetSize() << std::endl;

        std::unique_ptr<ReportGenerator> team_report = std::make_unique<TeamReportGenerator>(red_);
        team_report->GenerateReport();
    }
}

void Game::SpecAction(Team *l_team, Team *r_team, int was_killed, int start_pos) {
    std::string msg = "\n[" + l_team->GetTeamName() + "] use SpecActions via [" + r_team->GetTeamName() + "]\n";
    std::cout << msg;
    logger_.AddLogMsg(msg, LogType::SPEC_ACT);
    for (int i = (was_killed) ? 0+start_pos : 1+start_pos; i < l_team->GetSize(); ++i) {
        IUnit *unit = l_team->GetUnitByPos(i);
        // TODO: как применять специальные действия
        if (ExtractTypeFromUnitPtr(unit) == "HeavyHero") {
            msg = "[" + ExtractTypeFromUnitPtr(unit) + "] [index=" + std::to_string(l_team->GetHeroNumber(unit)) +
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
                msg = "[Hiller] [index=" + std::to_string(l_team->GetHeroNumber(unit)) + "] hills... " +
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
                    l_team->AddUnitToPos(cloned_unit, pos);
                    if (pos <= i) {
                        i++;
                    }
                }
            }
        } else if (ExtractTypeFromUnitPtr(unit) == "Wagenburg") {
            msg = "[" + ExtractTypeFromUnitPtr(unit) + "] [index=" + std::to_string(l_team->GetHeroNumber(unit)) +
                  "] Does not have special ability\n";
            std::cout << msg;
            logger_.AddLogMsg(msg, LogType::SPEC_ACT);
        }
    }
    logger_.ExecLog(LogType::SPEC_ACT);
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
    } else if (input == "u") {
        if (!game.UndoLastTurn()) {
            std::cout << "Nothing to undo!" << std::endl;
        }
        std::cout << "Enter 'n' for next turn or 'u' to undo last turn (max 3): ";
        std::cin >> input;
        HandleInput(game, input);
    } else {
        std::cout << "Error. Enter 'n' again: ";
        std::cin >> input;
        HandleInput(game, input);
    }
}

// Обработка состояния хода
void ProcessingTurnState::Update(Game &game) {
    game.ProcessTurnLogic();
    game.SetState(CreateWaitingState());
}

void ProcessingTurnState::HandleInput(Game &game, std::string &input) {
    // Игнорируем ввод
}
