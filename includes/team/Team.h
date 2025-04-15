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

/*
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
        auto it = hero_numbers_.find(hero);
        if (it != hero_numbers_.end()) {
            return it->second;
        }
        return 0;  // или бросить исключение, если герой не найден
    }

    // Удалить героя из системы (но номер останется занятым)
    void RemoveHero(IUnit* hero) {
        hero_numbers_.erase(hero);
    }

    // Проверить, есть ли номер у героя
    bool HasNumber(IUnit* hero) const {
        return hero_numbers_.count(hero) > 0;
    }
};

class Team {
public:
    Team(const std::string& team_name) : team_name_(team_name) {}

    // Добавляем героя с автоматическим назначением номера
    void AddUnit(IUnit* unit) {
        if (!number_manager_.HasNumber(unit)) {
            number_manager_.AssignNumber(unit);
        }
        units_.push_back(unit);
    }

    // Добавляем героя на конкретную позицию с сохранением номера
    void AddUnitToPos(IUnit* unit, unsigned int pos) {
        if (pos >= units_.size()) {
            units_.resize(pos + 1, nullptr);
        }
        if (!number_manager_.HasNumber(unit)) {
            number_manager_.AssignNumber(unit);
        }
        units_[pos] = unit;
    }

    // Получить номер героя
    unsigned int GetHeroNumber(IUnit* unit) const {
        return number_manager_.GetNumber(unit);
    }

    // Удаление героя из команды (но номер сохраняется)
    IUnit* GetUnit() {
        if (units_.empty()) return nullptr;
        IUnit* unit = units_.front();
        units_.pop_front();
        return unit;
    }

    // ... остальные методы остаются без изменений

private:
    std::deque<IUnit*> units_;
    std::string team_name_;
    static HeroNumberManager number_manager_;  // статический, если номера общие для всех команд
};

 */

std::string ExtractTypeFromUnitPtr(IUnit* unit);
std::string ExtractTHeavyHeroypeFromUnitPtr(HeavyHero* unit);
#endif //SBERGAME_TEAM_H
