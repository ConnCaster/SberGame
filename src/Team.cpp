#include <Team.h>

const std::unordered_map<std::string, unsigned int> kHeroCosts {
        {"Hero",        20},
        {"HeavyHero",   30},
        {"Archer",      10},
        {"Hiller",      20},
        {"Wizard",      20},
};
constexpr unsigned int kMaxTeamCost = 150;
constexpr unsigned int kMinUnitCost = 10;

Team::Team()
    : units_{}, team_cost_{0}
{
    GenerateTeam();
}

Team::~Team() {
    for (auto unit: units_) {
        delete unit;
    }
}

IUnit* Team::GetUnit() {
    if (!units_.empty()) {
        IUnit* unit = units_.front();
        units_.pop_front();
        return unit;
    }
    return nullptr;
}

void Team::ReturnUnit(IUnit *unit) {
    units_.push_back(unit);
}

void Team::GenerateTeam() {
    while (team_cost_ <= (kMaxTeamCost - kMinUnitCost)) {
        int random_unit_idx = std::rand() % kHeroCosts.size();
        auto it = kHeroCosts.begin();
        std::advance(it, random_unit_idx);
        unsigned int unit_cost = it->second;
        if (team_cost_ + unit_cost > kMaxTeamCost) {
            continue;
        }
        IUnit* unit = UnitFactory::CreateUnit(it->first);
        units_.push_back(unit);
        team_cost_ += unit_cost;
    }
}
