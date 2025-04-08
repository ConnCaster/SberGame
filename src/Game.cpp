#include <iostream>

#include "Game.h"

Game* Game::game_instance_ = nullptr;

Game* Game::GetInstance() {
    if (!game_instance_) {
        game_instance_ = new Game{};
    }
    return game_instance_;
}

Game::Game()
    : red_{nullptr}, blue_{nullptr}, red_team_builder_{nullptr}, blue_team_builder_{nullptr}
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

    bool red_team_order = true;
    while (red_ && blue_ && !red_->IsEmpty() && !blue_->IsEmpty()) {
        std::cout << "=====================================" << std::endl;
        IUnit* red_unit = red_->GetUnit();
        IUnit* blue_unit = blue_->GetUnit();
        if (red_team_order) {
            // герой наносит удар
            int is_killed_blue = Attack(red_unit, red_, blue_unit, blue_);
            int is_killed_red{0};
            if (!is_killed_blue) {
                // если противник выжил, то он отвечает
                red_unit = red_->GetUnit();
                blue_unit = blue_->GetUnit();
                is_killed_red = Attack(blue_unit, blue_, red_unit, red_);
            }
            SpecAction(red_, blue_, is_killed_red);
            SpecAction(blue_, red_, is_killed_blue);
            red_team_order = false;
        } else {
            // герой наносит удар
            int is_killed_red = Attack(blue_unit, blue_, red_unit, red_);
            int is_killed_blue{0};
            if (!is_killed_red) {
                // если противник выжил, то он отвечает
                red_unit = red_->GetUnit();
                blue_unit = blue_->GetUnit();
                is_killed_blue = Attack(red_unit, red_, blue_unit, blue_);
            }
            SpecAction(blue_, red_, is_killed_blue);
            SpecAction(red_, blue_, is_killed_red);
            red_team_order = true;
        }
    }
    ShowGameResults();
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
int Attack(IUnit* l, Team* l_team, IUnit* r, Team* r_team) {
    std::cout << "[" << l_team->GetTeamName() << "] ";
    if (typeid(*l) == typeid(HeavyHero)) {
        std::cout << "HeavyHero attacks... " << ExtractTypeFromUnitPtr(r) << std::endl;
//        if (ExtractTypeFromUnitPtr(l_team->GetUnitByPos(0)) == "Hero") {
//            l = AppendBuffToHeavyHero(dynamic_cast<HeavyHero*>(l));
//        }
        dynamic_cast<HeavyHero*>(l)->PerformAttack(r);
    } else if (typeid(*l) == typeid(Hero)) {
        std::cout << "Hero attacks... " << ExtractTypeFromUnitPtr(r) << std::endl;
        dynamic_cast<Hero*>(l)->PerformAttack(r);
    } else if (typeid(*l) == typeid(Archer)) {
        std::cout << "Archer attacks... " << ExtractTypeFromUnitPtr(r) << std::endl;

        r_team->ReturnUnit(r);
        IUnit* far_unit = r_team->GetRandomUnit(dynamic_cast<Archer*>(l)->distance_);
        if (far_unit) {
            dynamic_cast<Archer*>(l)->PerformAttack(far_unit);
            r = far_unit;
        }

    } else if (typeid(*l) == typeid(Hiller)) {
        std::cout << "Hiller attacks... " << ExtractTypeFromUnitPtr(r) << std::endl;
        dynamic_cast<Hiller*>(l)->PerformAttack(r);
    } else if (typeid(*l) == typeid(Wizard)) {
        std::cout << "Wizard clone... " << ExtractTypeFromUnitPtr(l) << std::endl;
        IUnit* red_cloned_unit = dynamic_cast<Wizard*>(l)->PerformSpecAction(l);
        if (red_cloned_unit) {
            l_team->AddUnit(red_cloned_unit);
        }
    } else if (typeid(*l) == typeid(WagenburgAdapter)) {
        std::cout << "Wageeeeenbuuuuuurggg" << std::endl;
    }
    l_team->ReturnUnit(l);
    if (r && r->GetHealth() == 0) {
        std::cout << "\tFINISH HIM!!!" << std::endl;
        delete r;
        return 1;
    } else {
        if (r) {
            r_team->ReturnUnit(r);
        }
    }
    return 0;
}

void SpecAction(Team* l_team, Team* r_team, int was_killed) {
    std::cout << "[" << l_team->GetTeamName() << "] use SpecActions via [" << r_team->GetTeamName() << "]\n";
    for (int i = (was_killed) ? 1 :0; i < l_team->GetSize(); ++i) {
        IUnit* unit = l_team->GetUnitByPos(i);
        std::cout << "[" << ExtractTypeFromUnitPtr(unit) << "]" << std::endl;
    }

}
