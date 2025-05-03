#include <deque>
#include <map>
#include <string>
#include <iostream>
#include <set>

#include "team/TeamBuilder.h"

const std::map<std::string, unsigned int> kHeroCosts {
    {"Hero",        50},
    {"HeavyHero",   60},
    {"Archer",      30},
    {"Hiller",      10},
    {"Wizard",      20},
    {"Wagenburg",   40}
};
constexpr unsigned int kMinUnitCost = 10;

void TeamBuilderRandom::GenerateTeam() {
    while (team_cost_ <= (team_cost_max_ - kMinUnitCost)) {
        int random_unit_idx = std::rand() % kHeroCosts.size();
        auto it = kHeroCosts.begin();
        std::advance(it, random_unit_idx);
        unsigned int unit_cost = it->second;
        if (team_cost_ + unit_cost > team_cost_max_) {
            continue;
        }
        IUnit* unit = UnitFactory::CreateUnit(it->first);
        team_->AddUnit(unit);
        team_cost_ += unit_cost;
    }
}

Team* TeamBuilderRandom::GetTeam() {
    std::cout << team_->GetTeamInfo() <<
                 "Cost: " + std::to_string(team_cost_) << "\n\n";
    return team_;
}

void TeamBuilderGreedy::GenerateTeam() {
    auto comp = [](const std::pair<std::string, unsigned int> &l, const std::pair<std::string, unsigned int> &r) {
        return l.second > r.second;
    };
    std::set<std::pair<std::string, unsigned int>, decltype(comp)> pairs(kHeroCosts.begin(), kHeroCosts.end(), comp);
    while (team_cost_ <= (team_cost_max_ - kMinUnitCost) && !pairs.empty()) {
        auto it  = pairs.begin();
        pairs.erase(it);
        unsigned int unit_cost = it->second;

        while (true) {
            if (team_cost_ + unit_cost > team_cost_max_) {
                break;
            }
            IUnit *unit = UnitFactory::CreateUnit(it->first);
            team_->AddUnit(unit);
            team_cost_ += unit_cost;
        }
    }
}

Team* TeamBuilderGreedy::GetTeam() {
    std::cout << team_->GetTeamInfo() <<
                 "Cost: " + std::to_string(team_cost_) << std::endl;
    return team_;
}

IUnit* UnitSelectorByHands::SelectHero() {
    std::string hero_name;

    std::cout << "Enter unit name [Hero=50/HeavyHero=60/Archer=30/Hiller=10/Wizard=20/Wagenburg=40]: ";
    while (true) {
        std::cin >> hero_name;
        if (kHeroCosts.contains(hero_name)) {
            std::cout << "Unit is choose correctly" << std::endl;
            return UnitFactory::CreateUnit(hero_name);
        } else {
            std::cout << "Unknown unit name. Choose another one [Hero=50/HeavyHero=60/Archer=30/Hiller=10/Wizard=20/Wagenburg=40]: ";
        }
    }
}

void TeamBuilderByHandChoose::GenerateTeam() {
    std::cout << "Building team..." << std::endl;

    while (team_cost_ <= (team_cost_max_ - kMinUnitCost)) {
        IUnit* unit = selector_impl_->SelectHero();
        if (unit) {
            unsigned int cost{0};
            try {
                cost = kHeroCosts.at(ExtractTypeFromUnitPtr(unit));
            } catch (...) {
                std::cout << "Error getting unit cost" << std::endl;
                continue;
            }
            if (team_cost_ + cost <= team_cost_max_) {
                team_->AddUnit(unit);
                team_cost_ += cost;
            } else {
                std::cout << "Cannot add hero. Team cost exceeds maximum allowed. Choose another one" << std::endl;
                delete unit;
            }
        }
    }
    std::cout << "Team cost is out" << std::endl;
}

Team* TeamBuilderByHandChoose::GetTeam() {
    std::cout << team_->GetTeamInfo() <<
                 "Cost: " + std::to_string(team_cost_) << std::endl;
    return team_;
}