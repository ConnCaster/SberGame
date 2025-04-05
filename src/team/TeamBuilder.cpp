#include <deque>
#include <map>
#include <string>
#include <iostream>
#include <set>

#include "team/TeamBuilder.h"

const std::map<std::string, unsigned int> kHeroCosts {
    {"Hero",        40},
    {"HeavyHero",   50},
    {"Archer",      30},
    {"Hiller",      10},
    {"Wizard",      20},
    {"Wagenburg",   60}
};
constexpr unsigned int kMinUnitCost = 10;

void TeamBuilderRandom::GenerateTeam() {
    while (team_cost_ <= (team_cost_max - kMinUnitCost)) {
        int random_unit_idx = std::rand() % kHeroCosts.size();
        auto it = kHeroCosts.begin();
        std::advance(it, random_unit_idx);
        unsigned int unit_cost = it->second;
        if (team_cost_ + unit_cost > team_cost_max) {
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
    while (team_cost_ <= (team_cost_max - kMinUnitCost) && !pairs.empty()) {
        auto it  = pairs.begin();
        pairs.erase(it);
        unsigned int unit_cost = it->second;

        while (true) {
            if (team_cost_ + unit_cost > team_cost_max) {
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
