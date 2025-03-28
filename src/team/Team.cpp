#include <team/Team.h>

Team::Team()
    : units_{}
{}

Team::~Team() {
    for (auto unit: units_) {
        delete unit;
    }
}

IUnit* Team::GetUnit() {
    if (!units_.empty()) {
        IUnit* unit = units_.front();
        units_.pop_front();
        return unit;
    }
    return nullptr;
}

void Team::ReturnUnit(IUnit *unit) {
    units_.push_back(unit);
}

void Team::AddUnit(IUnit* unit) {
    units_.push_back(unit);
}

// TODO: пока работает для получения юнита команды противника
// для своей команды не работает из-за проблем со сдвигом distance
IUnit* Team::GetRandomUnit(unsigned int distance) {
    unsigned int randox_idx = 0;
    if (units_.empty()) return nullptr;
    if (distance >= units_.size()) {
        randox_idx = std::rand() % units_.size();
    } else {
        randox_idx = (distance) ? std::rand() % distance : 1;
    }
    unsigned int front_back = std::rand() % 2;
    std::deque<IUnit*>::iterator it;
    if (front_back == 0) {
        // берем от начала команды
        it = units_.begin();
        it += randox_idx;
    } else {
        // берем с конца
        it = units_.end() - 1;
        it -= randox_idx;
    }
    IUnit* u = *it;
    units_.erase(it);
    return u;
}

std::string ExtractTypeFromUnitPtr(IUnit* unit) {
    if (typeid(*unit) == typeid(HeavyHero)) return "HeavyHero";
    if (typeid(*unit) == typeid(Hero)) return "Hero";
    if (typeid(*unit) == typeid(Archer)) return "Archer";
    if (typeid(*unit) == typeid(Hiller)) return "Hiller";
    if (typeid(*unit) == typeid(Wizard)) return "Wizard";
}


