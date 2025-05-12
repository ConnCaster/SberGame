#ifndef SBERGAME_HERO_H
#define SBERGAME_HERO_H

#include <memory>

#include "interfaces/IAttack.h"
#include "interfaces/IUnit.h"
#include "interfaces/ICanBeCloned.h"
#include "interfaces/ICanBeHilled.h"

class Hero : public IUnit, public ICanBeHilled, public ICanBeCloned {
public:
    Hero();
    ~Hero() override = default;

    Hero(const Hero& other);

    IUnit* DeepCopy() {
        return new Hero(*this);
    }

    IUnit* Clone() override;
    void Hill(unsigned int additional_health) override;

    void DecreaseHealth(unsigned int damage) override;

    // атака и спецвозможности ...
    void SetAttack(std::unique_ptr<IAttack> attack);
    void PerformAttack(IUnit *target);
private:
    std::unique_ptr<IAttack> attack_;
};

#endif //SBERGAME_HERO_H
