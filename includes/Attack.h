#ifndef SBERGAME_ATTACK_H
#define SBERGAME_ATTACK_H

#include "interfaces/IUnit.h"
#include "interfaces/ITeam.h"
#include "logger/Logger.h"

class AttackMediator {
public:
    virtual int Attack(IUnit* l, IUnit* r) = 0;
    virtual ~AttackMediator() = default;
};

class UnitToUnitAttackMediator : public AttackMediator {
public:
    UnitToUnitAttackMediator(ITeam* l_team, ITeam* r_team, LogMsgHandler* logger)
        :l_team_{l_team}, r_team_{r_team}, logger_{logger}
    {}

    ~UnitToUnitAttackMediator() override = default;
    int Attack(IUnit* l, IUnit* r);

private:
    ITeam* l_team_;
    ITeam* r_team_;

    LogMsgHandler* logger_;
};

// @brief Pattern Facade
class AttackFacade {
public:
    AttackFacade(AttackMediator* attack_mediator)
        : attack_mediator_{attack_mediator}
    {}

    int Attack(IUnit* l, IUnit* r) {
        return attack_mediator_->Attack(l, r);
    }

    ~AttackFacade() {
        if (attack_mediator_) {
            delete attack_mediator_;
        }
    }

private:
    AttackMediator* attack_mediator_;
};

#endif //SBERGAME_ATTACK_H
