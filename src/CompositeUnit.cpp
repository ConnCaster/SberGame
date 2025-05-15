#include "interfaces/IUnit.h"

// Реализация методов CompositeUnit
CompositeUnit::CompositeUnit() : IUnit(0, 0, 0) {}

void CompositeUnit::DecreaseHealth(unsigned int damage) {
    for (IUnit* unit : units_) {
        unit->DecreaseHealth(damage);
    }
}

unsigned int CompositeUnit::GetHealth() const {
    unsigned int total_health = 0;
    for (IUnit* unit : units_) {
        total_health += unit->GetHealth();
    }
    return total_health;
}

std::string CompositeUnit::GetInfo() const {
    std::string info = "Composite Unit:\n";
    for (IUnit* unit : units_) {
        info += unit->GetInfo() + "\n";
    }
    return info;
}

bool CompositeUnit::IsAlive() const {
    for (IUnit* unit : units_) {
        if (unit->IsAlive()) {
            return true;
        }
    }
    return false;
}

void CompositeUnit::AddUnit(IUnit* unit) {
    units_.push_back(unit);
}

void CompositeUnit::RemoveUnit(IUnit* unit) {
    units_.erase(std::remove(units_.begin(), units_.end(), unit), units_.end());
}

IUnit* CompositeUnit::DeepCopy() {
    CompositeUnit* copy = new CompositeUnit();
    for (IUnit* unit : units_) {
        copy->AddUnit(unit->DeepCopy());
    }
    return copy;
} 