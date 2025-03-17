#ifndef SBERGAME_HEAVYHERO_H
#define SBERGAME_HEAVYHERO_H

#include <iostream>
#include <memory>

#include "interfaces/IAttack.h"
#include "interfaces/IUnit.h"

class HeavyHero : public IUnit {
public:
    HeavyHero();

    HeavyHero(unsigned int health, unsigned int protection, unsigned int damage);

    ~HeavyHero() override = default;

    void DecreaseHealth(unsigned int damage) override;
    inline bool IsHillable() const override { return false; };

    // атака и спецвозможности ...
    void SetAttack(std::unique_ptr<IAttack> attack);
    void PerformAttack(IUnit *target);

private:
    std::unique_ptr<IAttack> attack_;
};

#endif //SBERGAME_HEAVYHERO_H
