#ifndef SBERGAME_NORMALATTACK_H
#define SBERGAME_NORMALATTACK_H

#include "interfaces/IAttack.h"
#include "interfaces/IUnit.h"

class NormalAttack : public IAttack {
public:
    ~NormalAttack() override = default;

    void DoAttack(IUnit* target, unsigned int damage) override;
};

#endif //SBERGAME_NORMALATTACK_H
