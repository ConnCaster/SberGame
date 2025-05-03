#ifndef SBERGAME_ITEAM_H
#define SBERGAME_ITEAM_H

#include <deque>

class ITeam {
public:
    virtual ~ITeam() = default;
    virtual void AddUnit(IUnit* unit) = 0;
    virtual IUnit* GetUnit() = 0;
    virtual IUnit* GetRandomUnit(unsigned int distance = 0) = 0;
    virtual void ReturnUnit(IUnit* unit) = 0;
    virtual IUnit* GetUnitByPos(unsigned int pos) = 0;

    virtual bool IsEmpty() const = 0;
    virtual unsigned int GetSize() const = 0;
    virtual std::string GetTeamInfo() const = 0;
    virtual std::string GetTeamName() const = 0;
    virtual std::deque<IUnit*> GetTeam() = 0;

    virtual unsigned int GetHeroNumber(IUnit* unit) const = 0;
};


#endif //SBERGAME_ITEAM_H
