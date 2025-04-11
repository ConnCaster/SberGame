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
    while (team_cost_ <= (team_cost_max - kMinUnitCost)) {
        int random_unit_idx = std::rand() % kHeroCosts.size();
        auto it = kHeroCosts.begin();
        std::advance(it, random_unit_idx);
        unsigned int unit_cost = it->second;
        if (team_cost_ + unit_cost > team_cost_max) {
            continue;
        }
        IUnit* unit = UnitFactory::CreateUnit(it->first);
        /*
        // Проверка необходимости применения усилителей
        // если добавляемый герой HeavyHero, то сразу применяем усилители, а затем добавляем героя в команду
        if (ExtractTypeFromUnitPtr(unit) == "HeavyHero") {
            if (team_->GetSize() > 0) {
                IUnit* prev_hero = team_->GetUnitByPos(team_->GetSize() - 1);
                if (ExtractTypeFromUnitPtr(prev_hero) == "Hero") {
                    HeavyHero* heavy_hero_with_buff = AppendBuffToHeavyHero(dynamic_cast<HeavyHero*>(unit));
                    team_->AddUnit(heavy_hero_with_buff);
                    std::cout << "BUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUF" << std::endl;
                } else {
                    team_->AddUnit(unit);
                }
            } else {
                team_->AddUnit(unit);
            }
        } else {*/
        // проверяем, не был ли предыдущий герой HeavyHero
        team_->AddUnit(unit);
        if (ExtractTypeFromUnitPtr(unit) == "Hero") {
            IUnit* heavy_hero = team_->CheckIfHeavyHeroNeighbour(team_->GetSize() - 1);
            if (heavy_hero) {
                HeavyHero* heavy_hero_with_buff = AppendBuffToHeavyHero(dynamic_cast<HeavyHero*>(heavy_hero));
                // заменяем предпоследнего героя на героя с бафами
                team_->ReplaceUnit(heavy_hero_with_buff, team_->GetSize() - 2);
            }
        }
//        }
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
            if (ExtractTypeFromUnitPtr(unit) == "Hero") {
                IUnit* heavy_hero = team_->CheckIfHeavyHeroNeighbour(team_->GetSize() - 1);
                if (heavy_hero) {
                    HeavyHero* heavy_hero_with_buff = AppendBuffToHeavyHero(dynamic_cast<HeavyHero*>(heavy_hero));
                    // заменяем предпоследнего героя на героя с бафами
                    team_->ReplaceUnit(heavy_hero_with_buff, team_->GetSize() - 2);
                }
            }
            team_cost_ += unit_cost;
        }
    }
}

Team* TeamBuilderGreedy::GetTeam() {
    std::cout << team_->GetTeamInfo() <<
                 "Cost: " + std::to_string(team_cost_) << std::endl;
    return team_;
}
