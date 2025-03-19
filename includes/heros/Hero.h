#ifndef SBERGAME_HERO_H
#define SBERGAME_HERO_H

#include <iostream>
#include <memory>

#include "interfaces/IAttack.h"
#include "interfaces/IUnit.h"

class Hero : public IUnit {
public:
    Hero();
    ~Hero() override = default;

    IUnit* Clone() override { return new Hero{}; };

    void DecreaseHealth(unsigned int damage) override;
    void IncreaseHealth(unsigned int additional_health) override;

    // атака и спецвозможности ...
    void SetAttack(std::unique_ptr<IAttack> attack);
    void PerformAttack(IUnit *target);
private:
    std::unique_ptr<IAttack> attack_;
};

#endif //SBERGAME_HERO_H
