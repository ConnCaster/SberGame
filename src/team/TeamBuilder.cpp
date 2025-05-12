#include <deque>
#include <map>
#include <string>
#include <iostream>
#include <set>
#include <fstream>

#include "team/TeamBuilder.h"

#include <filesystem>

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
            std::cout << "Unknown unit name. Choose again [Hero=50/HeavyHero=60/Archer=30/Hiller=10/Wizard=20/Wagenburg=40]: ";
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

void TeamBuilderAllUnits::GenerateTeam() {
    auto it = kHeroCosts.begin();
    while (team_cost_ <= (team_cost_max_ - kMinUnitCost) && it != kHeroCosts.end()) {
        unsigned int unit_cost = it->second;
        if (team_cost_ + unit_cost > team_cost_max_) {
            std::cout << "All units mode is more expensive then max team cost. Abort..." << std::endl;
            return;
        }
        IUnit* unit = UnitFactory::CreateUnit(it->first);
        team_->AddUnit(unit);
        team_cost_ += unit_cost;
        it++;
    }
    if (it == kHeroCosts.end()) {
        all_units_flag = true;
    }

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

Team* TeamBuilderAllUnits::GetTeam() {
    std::cout << team_->GetTeamInfo() <<
                 "Cost: " + std::to_string(team_cost_) << std::endl;
    return (all_units_flag) ? team_ : nullptr;
}

void TeamBuilderFromFile::GenerateTeam() {
    std::string file_path{};
    std::cout << "Enter path to file with units: ";
    std::cin >> file_path;
    if (!std::filesystem::exists(std::filesystem::path(file_path))) {
        enough_money = false;
        return;
    }
    std::ifstream file_of_units{file_path};
    std::vector<std::string> units_names{};
    while (file_of_units) {
        std::string unit_name{};
        file_of_units >> unit_name;
        if (unit_name.empty()) continue;
        if (!kHeroCosts.contains(unit_name)) {
            enough_money = false;
            file_of_units.close();
            return;
        }
        units_names.push_back(unit_name);
    }
    auto it = units_names.begin();
    while (team_cost_ <= (team_cost_max_ - kMinUnitCost) && it != units_names.end()) {
        unsigned int unit_cost = kHeroCosts.at(*it);
        if (team_cost_ + unit_cost > team_cost_max_) {
            std::cout << "All units mode is more expensive then max team cost. Abort..." << std::endl;
            enough_money = false;
            file_of_units.close();
            return;
        }
        IUnit* unit = UnitFactory::CreateUnit(*it);
        team_->AddUnit(unit);
        team_cost_ += unit_cost;
        it++;
    }
    if (it != units_names.end()) {
        enough_money = false;
    }
    file_of_units.close();
}

Team* TeamBuilderFromFile::GetTeam() {
    std::cout << team_->GetTeamInfo() <<
                 "Cost: " + std::to_string(team_cost_) << std::endl;
    return enough_money ? team_ : nullptr;
}