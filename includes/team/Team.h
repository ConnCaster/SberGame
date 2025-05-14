#ifndef SBERGAME_TEAM_H
#define SBERGAME_TEAM_H

#include <deque>
#include <string>

#include "interfaces/IUnit.h"
#include "interfaces/ITeam.h"
#include "UnitFactory.h"
#include "team/TeamIterator.h"
#include "team/UnitNumberManager.h"

std::string ExtractTypeFromUnitPtr(IUnit* unit);

class IUnitNumberManager;

// @brief Pattern ObjectPool
class Team : public ITeam {
public:
    Team(const std::string& team_name);
    ~Team() override;

    Team(const Team& other);

    TeamIterator* CreateIterator();

    // работа с командой юнитов: получение и возрат юнитов во время боя
    IUnit* GetUnit();  // удаляет игрока из команды
    IUnit* GetUnitByPosAndRemove(unsigned int pos); // удаляет из команды
    IUnit* GetRandomUnit(unsigned int distance = 0); // удаляет игрока из команды
    IUnit* GetUnitByPos(unsigned int pos);  // не удаляет игрока из команды
    void ReturnUnit(IUnit* unit);
    void ReturnUnitToPos(IUnit *unit, unsigned int pos);
    void ReplaceUnit(IUnit* unit, unsigned int pos);

    void RemoveUnit(IUnit *unit);

    void AddUnit(IUnit* unit);
    void AddUnitToPos(IUnit* unit, unsigned int pos);

    unsigned int GenPosAroundUnit(int pos, int distance) const;

    bool IsEmpty() const { return units_.empty(); }
    unsigned int GetSize() const { return units_.size(); }
    std::string GetTeamInfo() const;
    std::string GetTeamName() const { return team_name_; }
    unsigned int GetHeroNumber(IUnit* unit) const;

    std::deque<IUnit*> GetTeam() { return units_; }

    std::vector<std::pair<IUnit*, unsigned int>> CheckIfHeavyHeroNeighbour(unsigned int pos) const;

    void SetFormation(FormationType formation) {
        formation_ = formation;
        current_row_ = 0;
        current_unit_in_row_ = 0;
    }

    FormationType GetFormation() const {
        return formation_;
    }

    void ReSetCurrUnitInRow() override {
        current_unit_in_row_ = 0;
    }

// Метод для получения следующего атакующего юнита
    std::pair<IUnit*, int> GetNextAttacker() {
        if (units_.empty()) return {nullptr, 0};

        switch(formation_) {
            case FormationType::LINE_FIRST_ONLY:
                return {GetUnit(), 0};

            case FormationType::LINE_ALL_ATTACK: {
                for (size_t i = 0; i < units_.size(); ++i) {
                    int current = (current_unit_in_row_++) % units_.size();
                    if (units_[current]->IsAlive()) {
                        return {GetUnitByPosAndRemove(current), current};
                    }
                }
                return {nullptr, 0};
            }

            case FormationType::COLUMN_OF_THREE: {
                // Находим первую живую шеренгу
                while (current_row_ * 3 < units_.size()) {
                    // Проверяем, есть ли живые в текущей шеренге
                    bool row_has_alive = false;
                    for (int i = 0; i < 3 && (current_row_ * 3 + i) < units_.size(); ++i) {
                        if (units_[current_row_ * 3 + i]->IsAlive()) {
                            row_has_alive = true;
                            break;
                        }
                    }

                    if (row_has_alive) {
                        // Ищем следующего живого в шеренге
                        while (current_unit_in_row_ < 3 && (current_row_ * 3 + current_unit_in_row_) < units_.size()) {
                            IUnit* unit = units_[current_row_ * 3 + current_unit_in_row_];
                            current_unit_in_row_++;
                            if (unit->IsAlive()) {
                                return {unit, 0};
                            }
                        }

                        // Переходим к началу шеренги для следующего хода
                        current_unit_in_row_ = 0;
                        return GetNextAttacker();
                    } else {
                        // Переходим к следующей шеренге
                        current_row_++;
                        current_unit_in_row_ = 0;
                    }
                }

                // Все шеренги мертвы
                return {nullptr, 0};
            }
        }

        return {nullptr, 0};
    }

private:
    std::deque<IUnit*> units_;
    std::string team_name_;

    IUnitNumberManager* number_manager_;

    // Для колонны по три
    FormationType formation_;
    int current_row_ = 0;
    int current_unit_in_row_ = 0;
};
#endif //SBERGAME_TEAM_H
