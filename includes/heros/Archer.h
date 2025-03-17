#ifndef SBERGAME_ARCHER_H
#define SBERGAME_ARCHER_H

#include <iostream>
#include <memory>

#include "interfaces/IAttack.h"
#include "interfaces/IUnit.h"

class Archer : public IUnit {
public:
    Archer();
    Archer(unsigned int health, unsigned int protection, unsigned int damage, unsigned int distance);
    ~Archer() override = default;

    void DecreaseHealth(unsigned int damage) override;
    inline bool IsHillable() const override { return true; };
    void IncreaseHealth(unsigned int additional_health) override;

    // атака и спецвозможности ...
    void SetAttack(std::unique_ptr<IAttack> attack);
    void PerformAttack(IUnit *target);

private:
    unsigned int distance_;

    std::unique_ptr<IAttack> attack_;
};

#endif //SBERGAME_ARCHER_H
