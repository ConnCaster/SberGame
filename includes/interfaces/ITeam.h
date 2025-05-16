#ifndef SBERGAME_ITEAM_H
#define SBERGAME_ITEAM_H

#include <deque>

enum class FormationType {
    LINE_FIRST_ONLY,    // Линия, атакует только первый
    LINE_ALL_ATTACK,    // Линия, атакуют все
    COLUMN     // Колонна по три
};


class ITeam {
public:
    virtual ~ITeam() = default;
    virtual void AddUnit(IUnit* unit) = 0;
    virtual IUnit* GetUnit() = 0;
    virtual IUnit* GetRandomUnit(unsigned int distance = 0) = 0;
    virtual void ReturnUnit(IUnit* unit) = 0;
    virtual void ReturnUnitToPos(IUnit *unit, unsigned int pos) = 0;
    virtual IUnit* GetUnitByPos(unsigned int pos) = 0;
    virtual IUnit* GetUnitByPosAndRemove(unsigned int pos) = 0;
    virtual void RemoveUnit(IUnit *unit) = 0;

    virtual void ReSetCurrUnitInRow() = 0;

    virtual bool IsEmpty() const = 0;
    virtual unsigned int GetSize() const = 0;
    virtual std::string GetTeamInfo() const = 0;
    virtual std::string GetTeamName() const = 0;
    virtual std::deque<IUnit*> GetTeam() = 0;

    virtual FormationType GetFormation() const = 0;

    virtual unsigned int GetHeroNumber(IUnit* unit) const = 0;
};


#endif //SBERGAME_ITEAM_H
