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
    if (SetTeamGenerationType()) return;
    std::cout << "\n==== Start game ====\n";
    red_ = CreateTeam(red_team_builder_);
    blue_ = CreateTeam(blue_team_builder_);
    /* TODO: выбор первой атакующей команды - random
1) Атакующая команда определяется рандомно всегда
2) Если у принимающего остается хп, то возвращаемся к 1 этапу
3) Если у получающего заканчивается хп, то он выбывает, на его место встает следующий боец и очереди. Возвращаемся к 1 этапу.
     */
    bool red_team_order = true;
    while (red_ && blue_ && !red_->IsEmpty() && !blue_->IsEmpty()) {
        std::cout << "=====================================" << std::endl;
        IUnit* red_unit = red_->GetUnit();
        IUnit* blue_unit = blue_->GetUnit();
        if (red_team_order) {
            std::cout << "[Red] ";
            Fight(red_unit, red_, blue_unit, blue_);
            red_team_order = false;
        } else {
            std::cout << "[Blue] ";
            Fight(blue_unit, blue_, red_unit, red_);
            red_team_order = true;
        }
    }
    ShowGameResults();
}

int Game::SetTeamGenerationType() {
    delete red_team_builder_;
    delete blue_team_builder_;
    std::cout << "Enter team generation type:\n\t"
                 "1. Random\n\t"
                 "2. Greedy\n\t"
                 "3. ?????\n";

    unsigned int type{0};
    std::cin >> type;
    if(!IsBuildTypeCorrect(type)) {
        std::cout << "Unknown team generation type" << std::endl;
        return 1;
    }

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
        std::cout << "Past RED units: " << blue_->GetSize() << std::endl;
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
void Fight(IUnit* l, Team* l_team, IUnit* r, Team* r_team) {
    if (typeid(*l) == typeid(HeavyHero)) {
        std::cout << "HeavyHero attacks... " << ExtractTypeFromUnitPtr(r) << std::endl;
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
    if (r && r->GetHealth() == 0) {
        std::cout << "\tFINISH HIM!!!" << std::endl;
        delete r;
    } else {
        if (r) {
            r_team->ReturnUnit(r);
        }
    }
    l_team->ReturnUnit(l);
}
