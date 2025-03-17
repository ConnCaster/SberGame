#ifndef SBERGAME_IATTACK_H
#define SBERGAME_IATTACK_H

#include <vector>
#include "interfaces/IUnit.h"

class IAttack {
public:
    virtual void DoAttack(unsigned int damage) {};
    virtual void DoAttack(IUnit* target, unsigned int damage) = 0;
    virtual void DoAttack(std::vector<IUnit*>& targets, unsigned int damage) {};
    virtual ~IAttack() = default;
};


#endif //SBERGAME_IATTACK_H
