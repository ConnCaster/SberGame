#include <iostream>

#include "Game.h"

Game::Game()
    : red_{nullptr},
        blue_{nullptr},
        red_team_builder_{nullptr},
        blue_team_builder_{nullptr},
        logger_death_(new FileLogger("./logger_deaths.log"), LogEventType::LOG_DEATHS),
        logger_spec_acts_(new FileLogger("./logger_spec_acts.log"), LogEventType::LOG_SPEC_ACTS)
{}

Game::~Game() {
    delete red_;
    delete blue_;
    delete red_team_builder_;
    delete blue_team_builder_;
}

void Game::Run() {
    SetTeamGenerationType();
    std::cout << "\n==== Start game ====\n";
    red_ = CreateTeam(red_team_builder_);
    blue_ = CreateTeam(blue_team_builder_);

    std::cout << "Enter first command [red/blue]: ";
    std::string first_command{};
    do {
        std::cin >> first_command;
        if (first_command != "red" && first_command != "blue") {
            std::cout << "Unknown command. Try again [red/blue] ... ";
        }
    } while (first_command != "red" && first_command != "blue");

    red_team_order_ = (first_command == "red") ? true : false;
    while (red_ && blue_ && !red_->IsEmpty() && !blue_->IsEmpty()) {
        std::cout << "=====================================" << std::endl;
        NextTurn();  // основной ход
    }
    ShowGameResults();
}


void Game::ChangeState(std::unique_ptr<GameState> new_state) {
    current_state_ = std::move(new_state);
    current_state_->Enter(*this);
}

void Game::HandleInput(char input) {
    if (current_state_) {
        current_state_->HandleInput(*this, input);
    }
}

void WaitingForInputState::Enter(Game& game) {
    std::cout << "Waiting for input...\n";
}

void WaitingForInputState::Update(Game& game) {
    // Ничего не делаем, ждем ввода
}

void WaitingForInputState::HandleInput(Game& game, char input) {
    if (input == 'n') {
        game.ChangeState(GameState::CreateProcessingState());
    }
}

void Game::NextTurn() {
    IUnit* red_unit = red_->GetUnit();
    IUnit* blue_unit = blue_->GetUnit();
    if (red_team_order_) {
        // герой красных наносит удар
        int is_killed_blue = Attack(red_unit, red_, blue_unit, blue_);
        int is_killed_red{0};
        if (!is_killed_blue) {
            // если синий противник выжил, то он отвечает
            red_unit = red_->GetUnit();
            blue_unit = blue_->GetUnit();
            is_killed_red = Attack(blue_unit, blue_, red_unit, red_);
        }
        // применяются специальные действия
        SpecAction(red_, blue_, is_killed_red);
        SpecAction(blue_, red_, is_killed_blue);
        red_team_order_ = false;
    } else {
        // герой синих наносит удар
        int is_killed_red = Attack(blue_unit, blue_, red_unit, red_);
        int is_killed_blue{0};
        if (!is_killed_red) {
            // если красный противник выжил, то он отвечает
            red_unit = red_->GetUnit();
            blue_unit = blue_->GetUnit();
            is_killed_blue = Attack(red_unit, red_, blue_unit, blue_);
        }
        // применяются специальные действия
        SpecAction(blue_, red_, is_killed_blue);
        SpecAction(red_, blue_, is_killed_red);
        red_team_order_ = true;
    }
}


void ProcessingTurnState::Enter(Game& game) {
    std::cout << "Processing turn...\n";
}

void ProcessingTurnState::Update(Game& game) {
    // game.ProcessTurnLogic();
    game.ChangeState(GameState::CreateWaitingState());
}

void ProcessingTurnState::HandleInput(Game& game, char input) {
    // Игнорируем ввод
}

// Фабричные методы
std::unique_ptr<GameState> GameState::CreateWaitingState() {
    return std::make_unique<WaitingForInputState>();
}

std::unique_ptr<GameState> GameState::CreateProcessingState() {
    return std::make_unique<ProcessingTurnState>();
}


int Game::SetTeamGenerationType() {
    delete red_team_builder_;
    delete blue_team_builder_;

    unsigned int type = rand() % 2+1;
    std::cout << "Enter team maximum cost: ";
    unsigned int team_max_cost{0};
    std::cin >> team_max_cost;

    red_team_builder_ = ITeamBuilderFactoryM::CreateTeamBuilder(type, "Red", team_max_cost);
    blue_team_builder_ = ITeamBuilderFactoryM::CreateTeamBuilder(type, "Blue", team_max_cost);
    return 0;
}

Team* Game::CreateTeam(ITeamBuilder* team_builder) {
    Director director{team_builder};
    director.ConstructTeam();
    return team_builder->GetTeam();
}

void Game::ShowGameResults() const {
    std::cout << "\n==== STATISTICS =====\n";
    if (red_ && red_->IsEmpty()) {
        std::cout << "Blue team WIN!" << std::endl;
        std::cout << "Past units: " << blue_->GetSize() << std::endl;
    }
    else if (blue_ && blue_->IsEmpty()) {
        std::cout << "Red team WIN!" << std::endl;
        std::cout << "Past units: " << red_->GetSize() << std::endl;
    }
}

// TODO: доработать механику боя
//  1. Archer бьет на расстоянии (выбирать оппонента, получать расстояние до него, атаковать по возможности)
//       - выбирать оппонента и получить расстояние до него:
//       В Team сделан метод GetRandomUnit(), возвращающий итератор на юнит из команды и его позицию от начала команды
//       Этот метод можно использовать как для атаки Archer, так и для клонирования и лечения.
//       Работает с обоими командами.
//  2. Hiller лечит и бьет палкой (выбирать, кого лечить, вычислять расстояние, лечить, что за палка и когда??)
//  3. Wizard клонирует (...)
int Game::Attack(IUnit* l, Team* l_team, IUnit* r, Team* r_team) {
    // сбиваем защиту HeavyHero с вероятностью 50%
    if (ExtractTypeFromUnitPtr(r) == "HeavyHero") {
        std::string buff_type = ExtractTHeavyHeroypeFromUnitPtr(dynamic_cast<HeavyHero*>(r));
        if (buff_type == "HorseDecorator" ||
                buff_type == "SpearDecorator" ||
                buff_type == "ShieldDecorator" ||
                buff_type == "HelmetDecorator"
                ) {
            if ((rand() % 100) < 50) {
                std::cout << "[HeavyHero] Buff " << buff_type << " is losed" << std::endl;
                r = dynamic_cast<HeavyHeroDecorator*>(r)->RemoveBuff(buff_type);
            } else {
                std::cout << "[HeavyHero] Buff " << buff_type << " is saved" << std::endl;
            }
        }
    }
    std::cout << "[" << l_team->GetTeamName() << "] ";
    if (ExtractTypeFromUnitPtr(l) == "HeavyHero") {
        std::cout << "HeavyHero attacks... " << ExtractTypeFromUnitPtr(r) << std::endl;
        dynamic_cast<HeavyHero*>(l)->PerformAttack(r);
    } else if ((ExtractTypeFromUnitPtr(l) == "Hero")) {
        std::cout << "Hero attacks... " << ExtractTypeFromUnitPtr(r) << std::endl;
        dynamic_cast<Hero*>(l)->PerformAttack(r);
    } else if ((ExtractTypeFromUnitPtr(l) == "Archer")) {
        std::cout << "Archer attacks... " << ExtractTypeFromUnitPtr(r) << std::endl;

        r_team->ReturnUnit(r);
        IUnit* far_unit = r_team->GetRandomUnit(dynamic_cast<Archer*>(l)->distance_);
        if (far_unit) {
            dynamic_cast<Archer*>(l)->PerformAttack(far_unit);
            r = far_unit;
        }

    } else if ((ExtractTypeFromUnitPtr(l) == "Hiller")) {
        std::cout << "Hiller attacks... " << ExtractTypeFromUnitPtr(r) << std::endl;
        dynamic_cast<Hiller*>(l)->PerformAttack(r);
    } else if ((ExtractTypeFromUnitPtr(l) == "Wizard")) {
        std::cout << "Wizard does not have an attack ability" << std::endl;
    } else if ((ExtractTypeFromUnitPtr(l) == "Wagenburg")) {
        std::cout << "Wagenburg does not have an attack ability" << std::endl;
    }
    l_team->ReturnUnit(l);
    if (r && r->GetHealth() == 0) {
        std::cout << "\tFINISH HIM!!!" << std::endl;
        std::string msg = "[" + r_team->GetTeamName() + "] " + ExtractTypeFromUnitPtr(r) + " was killed\n";
        logger_death_.Log(msg);
        delete r;
        return 1;
    } else {
        if (r) {
            r_team->ReturnUnit(r);
        }
    }
    return 0;
}

void Game::SpecAction(Team* l_team, Team* r_team, int was_killed) {
    std::string msg = "[" + l_team->GetTeamName() + "] use SpecActions via [" + r_team->GetTeamName() + "]\n";
    std::cout << msg;
    logger_spec_acts_.Log(msg);
    for (int i = (was_killed) ? 0 : 1; i < l_team->GetSize(); ++i) {
        IUnit* unit = l_team->GetUnitByPos(i);
        // TODO: как применять специальные действия
        if (ExtractTypeFromUnitPtr(unit) == "HeavyHero") {
            msg = "[" + ExtractTypeFromUnitPtr(unit) + "] Does not have special ability\n";
            std::cout << msg;
            logger_spec_acts_.Log(msg);
        } else if (ExtractTypeFromUnitPtr(unit) == "Hero") {
            msg = "[" + ExtractTypeFromUnitPtr(unit) + "] Try to append buff to neighbour HeavyHero if he exists\n";
            std::cout << msg;
            logger_spec_acts_.Log(msg);

            // Добавляем усилители HeavyHero
            std::vector<std::pair<IUnit*, unsigned int>> heavy_heros_vec = l_team->CheckIfHeavyHeroNeighbour(l_team->GetSize() - 1);
            for (auto& [heavy_hero, pos] : heavy_heros_vec) {
                if (heavy_hero) {
                    HeavyHero* heavy_hero_with_buff = AppendBuffToHeavyHero(dynamic_cast<HeavyHero*>(heavy_hero));
                    // заменяем героя в его позиция на героя с бафами
                    l_team->ReplaceUnit(heavy_hero_with_buff, pos);
                }
            }

        } else if (ExtractTypeFromUnitPtr(unit) == "Archer") {
            msg = "[" + ExtractTypeFromUnitPtr(unit) + "] Does not have special ability\n";
            std::cout << msg;
            logger_spec_acts_.Log(msg);
        } else if (ExtractTypeFromUnitPtr(unit) == "Hiller") {
            msg = "[" + ExtractTypeFromUnitPtr(unit) + "] Try to hill unit from his team\n";
            std::cout << msg;
            logger_spec_acts_.Log(msg);
            // Лечим героя из своей команды (легкого, лучника)
            int pos = l_team->GenPosAroundUnit(i, dynamic_cast<Hiller*>(unit)->GetHillDistance());
            IUnit* unit_to_hill = l_team->GetUnitByPos(pos);
            if (dynamic_cast<ICanBeHilled*>(unit_to_hill) == nullptr) {
                // если лечить нельзя
                msg = "[" + ExtractTypeFromUnitPtr(unit_to_hill) + "] Does not have ability to be hilled\n";
                std::cout << msg;
                logger_spec_acts_.Log(msg);
            } else {
                msg = "Hiller hills... " + ExtractTypeFromUnitPtr(unit_to_hill) + "\n";
                std::cout << msg;
                logger_spec_acts_.Log(msg);
                dynamic_cast<Hiller*>(unit)->PerformSpecAction(unit_to_hill);
            }
        } else if (ExtractTypeFromUnitPtr(unit) == "Wizard") {
            msg = "[" + ExtractTypeFromUnitPtr(unit) + "] Try to clone unit from his team\n";
            std::cout << msg;
            logger_spec_acts_.Log(msg);
            // Клонирование героя из своей команды (легкого, лучника, хиллера)
            int pos = rand() % l_team->GetSize();
            IUnit* unit_to_clone = l_team->GetUnitByPos(pos);
            if (dynamic_cast<ICanBeCloned*>(unit_to_clone) == nullptr) {
                // если клонировать нельзя
                msg = "[" + ExtractTypeFromUnitPtr(unit_to_clone) + "] Does not have ability to be clonned\n";
                std::cout << msg;
                logger_spec_acts_.Log(msg);
            } else {
                msg = "Wizard clone... " + ExtractTypeFromUnitPtr(unit_to_clone) + "\n";
                std::cout << msg;
                logger_spec_acts_.Log(msg);
                IUnit* cloned_unit = dynamic_cast<Wizard*>(unit)->PerformSpecAction(unit_to_clone);
                if (cloned_unit) {
                    l_team->AddUnitToPos(cloned_unit, i++);
                }
            }
        } else if (ExtractTypeFromUnitPtr(unit) == "Wagenburg") {
        msg = "[" + ExtractTypeFromUnitPtr(unit) + "] Does not have special ability\n";
        std::cout << msg;
        logger_spec_acts_.Log(msg);
        }
    }
}
