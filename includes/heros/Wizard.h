#ifndef SBERGAME_WIZARD_H
#define SBERGAME_WIZARD_H

#include <iostream>
#include <memory>

#include "interfaces/ISpecAction.h"
#include "interfaces/IUnit.h"

class Wizard : public IUnit {
public:
    Wizard();
    ~Wizard() override = default;

    void DecreaseHealth(unsigned int damage) override;

    // атака и спецвозможности ...
    void SetSpecAction(std::unique_ptr<ISpecAction> spec_action);
    IUnit* PerformSpecAction(IUnit *target);

private:
    IUnit* Clone() override {};
    void IncreaseHealth(unsigned int additional_health) override {};

private:
    unsigned int distance_;

    std::unique_ptr<ISpecAction> spec_action_;
};


#endif //SBERGAME_WIZARD_H
