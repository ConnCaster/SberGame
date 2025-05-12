#ifndef SBERGAME_HILLER_H
#define SBERGAME_HILLER_H

#include <memory>

#include "interfaces/IAttack.h"
#include "interfaces/ISpecAction.h"
#include "interfaces/IUnit.h"
#include "interfaces/ICanBeCloned.h"

class Hiller : public IUnit, public ICanBeCloned {
public:
    Hiller();
    ~Hiller() override = default;

    Hiller(const Hiller& other);

    IUnit* DeepCopy() {
        return new Hiller(*this);
    }

    IUnit* Clone() override;

    void DecreaseHealth(unsigned int damage) override;

    // атака и спецвозможности ...
    void SetAttack(std::unique_ptr<IAttack> attack);
    void SetSpecAction(std::unique_ptr<ISpecAction> spec_action);
    void PerformAttack(IUnit *target);
    void PerformSpecAction(IUnit *target);

    unsigned int GetHillDistance() const { return distance_; }

private:
//    unsigned int power_;
    unsigned int distance_;

    std::unique_ptr<IAttack> attack_;
    std::unique_ptr<ISpecAction> spec_action_;
};

#endif //SBERGAME_HILLER_H
