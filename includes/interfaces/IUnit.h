#ifndef SBERGAME_IUNIT_H
#define SBERGAME_IUNIT_H

#include <string>
#include <vector>
#include <algorithm>

class IUnit {
public:
    virtual ~IUnit() = default;
    IUnit(unsigned int health, unsigned int protection, unsigned int damage)
        : health_{health}, protection_{protection}, damage_{damage}
    {}

    virtual IUnit* DeepCopy() = 0;

    // методы, доступные всем юнитам ...
    virtual void DecreaseHealth(unsigned int damage) = 0;
    virtual unsigned int GetHealth() const { return health_; }

    virtual std::string GetInfo() const {
        return {
           "Health: " + std::to_string(health_) + " "
           "Protection: " + std::to_string(protection_) + " "
           "Damage: " + std::to_string(damage_)
        };
    }

    virtual bool IsAlive() const {
        return health_ > 0;
    }

protected:
    unsigned int health_;
    unsigned int protection_;
    unsigned int damage_;
};

// @brief Pattern Composite
class CompositeUnit : public IUnit {
public:
    CompositeUnit();
    void DecreaseHealth(unsigned int damage) override;
    unsigned int GetHealth() const override;
    std::string GetInfo() const override;
    bool IsAlive() const override;
    void AddUnit(IUnit* unit);
    void RemoveUnit(IUnit* unit);
    IUnit* DeepCopy() override;

private:
    std::vector<IUnit*> units_;
};

#endif
