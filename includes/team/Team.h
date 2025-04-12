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
    Team(const std::string& team_name);
    ~Team();

    // работа с командой юнитов: получение и возрат юнитов во время боя
    IUnit* GetUnit();  // удаляет игрока из команды
    IUnit* GetRandomUnit(unsigned int distance = 0); // удаляет игрока из команды
    IUnit* GetUnitByPos(unsigned int pos);  // не удаляет игрока из команды
    void ReturnUnit(IUnit* unit);
    void ReplaceUnit(IUnit* unit, unsigned int pos);

    void AddUnit(IUnit* unit);
    void AddUnitToPos(IUnit* unit, unsigned int pos);

    unsigned int GenPosAroundUnit(int pos, int distance) const;

    bool IsEmpty() const { return units_.empty(); }
    unsigned int GetSize() const { return units_.size(); }
    std::string GetTeamInfo() const;
    std::string GetTeamName() const { return team_name_; }

    std::vector<std::pair<IUnit*, unsigned int>> CheckIfHeavyHeroNeighbour(unsigned int pos) const;

private:
    std::deque<IUnit*> units_;
    std::string team_name_;
};

std::string ExtractTypeFromUnitPtr(IUnit* unit);
std::string ExtractTHeavyHeroypeFromUnitPtr(HeavyHero* unit);
#endif //SBERGAME_TEAM_H
