#ifndef SBERGAME_ARCHER_H
#define SBERGAME_ARCHER_H

#include <iostream>
#include <memory>

#include "interfaces/IAttack.h"
#include "interfaces/IUnit.h"
#include "interfaces/ICanBeCloned.h"
#include "interfaces/ICanBeHilled.h"

class Archer : public IUnit, public ICanBeHilled, public ICanBeCloned {
public:
    Archer();
    ~Archer() override = default;

    IUnit* Clone() override { return new Archer{}; };
    void Hill(unsigned int additional_health) override;

    void DecreaseHealth(unsigned int damage) override;

    // атака и спецвозможности ...
    void SetAttack(std::unique_ptr<IAttack> attack);
    void PerformAttack(IUnit *target);

private:
    unsigned int distance_;

    std::unique_ptr<IAttack> attack_;
};

#endif //SBERGAME_ARCHER_H
