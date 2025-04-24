#ifndef SBERGAME_TEAM_H
#define SBERGAME_TEAM_H

#include <deque>
#include <unordered_map>
#include <string>

#include "interfaces/IUnit.h"
#include "interfaces/ITeam.h"
#include "UnitFactory.h"
#include "team/TeamIterator.h"

std::string ExtractTypeFromUnitPtr(IUnit* unit);

class HeroNumberManager {
private:
    std::unordered_map<IUnit*, unsigned int> hero_numbers_;
    unsigned int next_number_ = 1;  // начинаем нумерацию с 1

public:
    // Назначить новый номер герою
    unsigned int AssignNumber(IUnit* hero) {
        unsigned int number = next_number_++;
        hero_numbers_[hero] = number;
        return number;
    }

    // Получить номер героя
    unsigned int GetNumber(IUnit* hero) const {
        if (
            dynamic_cast<HeavyHero*>(hero) &&
            ExtractHeavyHeroTypeFromUnitPtr(dynamic_cast<HeavyHero*>(hero)) != "HeavyHero"
            ) {
            hero = dynamic_cast<HeavyHeroDecorator*>(hero)->GetInnerHeavyHeroOrigin();
        }
        auto it = hero_numbers_.find(hero);
        if (it != hero_numbers_.end()) {
            return it->second;
        }
        return 0;
    }

    // Удалить героя из системы (но номер останется занятым)
    void RemoveHero(IUnit* hero) {
        hero_numbers_.erase(hero);
    }

    // Заменить героя, сохранив его номер
    void ReplaceHero(IUnit* prev_hero, IUnit* new_hero) {
        if (
            dynamic_cast<HeavyHero*>(prev_hero) &&
            ExtractHeavyHeroTypeFromUnitPtr(dynamic_cast<HeavyHero*>(prev_hero)) == "HeavyHero"
            ) {
            return;
        }
        unsigned int prev_number = hero_numbers_[prev_hero];
        hero_numbers_.erase(prev_hero);
        hero_numbers_[new_hero] = prev_number;
    }

    // Проверить, есть ли номер у героя
    bool HasNumber(IUnit* hero) const {
        return hero_numbers_.count(hero) > 0;
    }
};

// @brief Pattern ObjectPool
class Team : public ITeam {
public:
    Team(const std::string& team_name);
    ~Team();

    TeamIterator* CreateIterator();

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
    unsigned int GetHeroNumber(IUnit* unit) const;

    std::vector<std::pair<IUnit*, unsigned int>> CheckIfHeavyHeroNeighbour(unsigned int pos) const;

private:
    std::deque<IUnit*> units_;
    std::string team_name_;

    HeroNumberManager number_manager_;
};
#endif //SBERGAME_TEAM_H
