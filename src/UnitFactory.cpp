#include "UnitFactory.h"

IUnit *UnitFactory::CreateUnit(const std::string& hero_type) {
    if (hero_type == "Hero") {
        return new Hero{};
    } else if (hero_type == "HeavyHero") {
        return new HeavyHero{};
    } else if (hero_type == "Archer") {
        return new Archer{};
    } else if (hero_type == "Hiller") {
        return new Hiller{};
    } else if (hero_type == "Wizard") {
        return new Wizard{};
    } else if (hero_type == "Wagenburg") {
        return new WagenburgAdapter{new Wagenburg{}};
    }
    // TODO: добавить нового героя
    return nullptr;
}
