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

    void DecreaseHealth(unsigned int damage) override;
    inline bool IsHillable() const override { return true; };
    void IncreaseHealth(unsigned int additional_health) override;

    // атака и спецвозможности ...
    void SetAttack(std::unique_ptr<IAttack> attack);
    void PerformAttack(IUnit *target);
private:
    std::unique_ptr<IAttack> attack_;
};

#endif //SBERGAME_HERO_H
