#ifndef SBERGAME_ARCHER_H
#define SBERGAME_ARCHER_H

#include <memory>

#include "interfaces/IAttack.h"
#include "interfaces/IUnit.h"
#include "interfaces/ICanBeCloned.h"
#include "interfaces/ICanBeHilled.h"

class Archer : public IUnit, public ICanBeHilled, public ICanBeCloned {
public:
    Archer();
    ~Archer() override = default;

    Archer(const Archer& other);

    IUnit* DeepCopy() {
        return new Archer(*this);
    }

    IUnit* Clone() override;
    void Hill(unsigned int additional_health) override;

    void DecreaseHealth(unsigned int damage) override;

    // атака и спецвозможности ...
    void SetAttack(std::unique_ptr<IAttack> attack);
    void PerformAttack(IUnit *target);

public:
    static unsigned int distance_;

private:
    std::unique_ptr<IAttack> attack_;
};

#endif //SBERGAME_ARCHER_H
