#ifndef SBERGAME_DISTANCEATTACK_H
#define SBERGAME_DISTANCEATTACK_H

#include "interfaces/IAttack.h"
#include "interfaces/IUnit.h"

class DistanceAttack : public IAttack {
public:
    ~DistanceAttack() override = default;

    void DoAttack(IUnit* target, unsigned int damage) override;
};


#endif //SBERGAME_DISTANCEATTACK_H
