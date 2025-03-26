#ifndef SBERGAME_TEAM_H
#define SBERGAME_TEAM_H

#include <deque>
#include <unordered_map>
#include <string>

#include "interfaces/IUnit.h"
#include "UnitFactory.h"

// @brief Pattern ObjectPool
class Team {
public:
    Team();
    ~Team();

    // работа с командой юнитов: получение и возрат юнитов для боя
    IUnit* GetUnit();
    void ReturnUnit(IUnit* unit);

    std::string GetInfo() const {
        return {
                "Team: " + std::to_string(units_.size()) + " units "
                "cost: " + std::to_string(team_cost_)
        };
    }

    bool IsEmpty() const { return units_.empty(); }

private:
    void GenerateTeam();

private:
    std::deque<IUnit*> units_;
    unsigned int team_cost_;
};

#endif //SBERGAME_TEAM_H
