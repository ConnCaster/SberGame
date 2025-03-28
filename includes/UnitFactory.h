#ifndef SBERGAME_UNITFACTORY_H
#define SBERGAME_UNITFACTORY_H

#include <unordered_map>
#include <string>

#include "interfaces/IUnit.h"
#include "heros/HeavyHero.h"
#include "heros/Hero.h"
#include "heros/Archer.h"
#include "heros/Hiller.h"
#include "heros/Wizard.h"
#include "heros/Wagenburg.h"

// @brief Pattern FactoryMethod
class UnitFactory {
public:
    static IUnit* CreateUnit(const std::string& hero_type);
};

#endif //SBERGAME_UNITFACTORY_H
