#ifndef SBERGAME_WIZARD_H
#define SBERGAME_WIZARD_H

#include <memory>

#include "interfaces/ISpecAction.h"
#include "interfaces/IUnit.h"
#include "spec_actions/CloneSpecAction.h"

class Wizard : public IUnit {
public:
    Wizard();
    ~Wizard() override = default;

    Wizard(const Wizard& other)
        : IUnit(other.health_, other.protection_, other.damage_), distance_(other.distance_), spec_action_{std::make_unique<CloneSpecAction>()}
    {}

    IUnit* DeepCopy() {
        return new Wizard(*this);
    }

    void DecreaseHealth(unsigned int damage) override;

    // атака и спецвозможности ...
    void SetSpecAction(std::unique_ptr<ISpecAction> spec_action);
    IUnit* PerformSpecAction(IUnit *target);

private:
    unsigned int distance_;

    std::unique_ptr<ISpecAction> spec_action_;
};


#endif //SBERGAME_WIZARD_H
