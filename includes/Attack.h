#ifndef SBERGAME_ATTACK_H
#define SBERGAME_ATTACK_H

#include "interfaces/IUnit.h"
#include "interfaces/ITeam.h"
#include "logger/Logger.h"

// @brief Pattern Facade
class AttackFacade {
public:
    AttackFacade(ITeam* l_team, ITeam* r_team, ILogger* logger)
        : l_team_{l_team}, r_team_{r_team}, logger_{logger}
    {}

    int Attack(IUnit* l, IUnit* r);

private:
    ITeam* l_team_;
    ITeam* r_team_;

    ILogger* logger_;
};

#endif //SBERGAME_ATTACK_H
