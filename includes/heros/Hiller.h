#ifndef SBERGAME_HILLER_H
#define SBERGAME_HILLER_H

#include <iostream>
#include <memory>

#include "interfaces/IAttack.h"
#include "interfaces/ISpecAction.h"
#include "interfaces/IUnit.h"

class Hiller : public IUnit {
public:
    Hiller();
    ~Hiller() override = default;

    void DecreaseHealth(unsigned int damage) override;
    inline bool IsHillable() const override { return false; };

    // атака и спецвозможности ...
    void SetAttack(std::unique_ptr<IAttack> attack);
    void SetSpecAction(std::unique_ptr<ISpecAction> spec_action);
    void PerformAttack(IUnit *target);
    void PerformSpecAction(IUnit *target);

private:
//    unsigned int power_;
    unsigned int distance_;

    std::unique_ptr<IAttack> attack_;
    std::unique_ptr<ISpecAction> spec_action_;
};

#endif //SBERGAME_HILLER_H
