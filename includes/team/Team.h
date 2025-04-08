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
    Team(const std::string team_name);
    ~Team();

    // работа с командой юнитов: получение и возрат юнитов во время боя
    IUnit* GetUnit();
    void ReturnUnit(IUnit* unit);

    void AddUnit(IUnit* unit);

    bool IsEmpty() const { return units_.empty(); }
    unsigned int GetSize() const { return units_.size(); }
    IUnit* GetRandomUnit(unsigned int distance = 0);
    IUnit* GetUnitByPos(unsigned int pos);

    std::string GetTeamInfo() const;
    std::string GetTeamName() const { return team_name_; }

private:
    std::deque<IUnit*> units_;
    std::string team_name_;
};

std::string ExtractTypeFromUnitPtr(IUnit* unit);


#endif //SBERGAME_TEAM_H
