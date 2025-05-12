#ifndef UNITNUMBERMANAGER_H
#define UNITNUMBERMANAGER_H

#include <unordered_map>

#include "interfaces/IUnit.h"
#include "team/Team.h"

class IUnitNumberManager {
public:
    virtual IUnitNumberManager* DeepCopy() = 0;
    virtual void ReplaceHero(IUnit* prev_hero, IUnit* new_hero) = 0;
    virtual bool HasNumber(IUnit* hero) const = 0;
    virtual unsigned int GetNumber(IUnit* hero) const = 0;
    virtual unsigned int AssignNumber(IUnit* hero) = 0;
};

class UnitNumberManager : public IUnitNumberManager {
public:
    UnitNumberManager() = default;

    // UnitNumberManager(const UnitNumberManager& other)
    //     : IUnitNumberManager(),
    //         hero_numbers_(other.hero_numbers_),
    //         next_number_(other.next_number_)
    // {}

    UnitNumberManager* DeepCopy() {
        return new UnitNumberManager(*this);
    }

    unsigned int AssignNumber(IUnit* hero);
    unsigned int GetNumber(IUnit* hero) const;

    // Удалить героя из системы (но номер останется занятым)
    inline void RemoveHero(IUnit* hero) {
        hero_numbers_.erase(hero);
    }

    // Заменить героя, сохранив его номер
    void ReplaceHero(IUnit* prev_hero, IUnit* new_hero);

    inline bool HasNumber(IUnit* hero) const {
        return hero_numbers_.count(hero) > 0;
    }

private:
    std::unordered_map<IUnit*, unsigned int> hero_numbers_;
    unsigned int next_number_ = 1;  // начинаем нумерацию с 1
};

#endif //UNITNUMBERMANAGER_H
