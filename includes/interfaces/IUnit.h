#ifndef SBERGAME_IUNIT_H
#define SBERGAME_IUNIT_H

class IUnit {
public:
    virtual ~IUnit() = default;
    IUnit(unsigned int health, unsigned int protection, unsigned int damage)
        : health_{health}, protection_{protection}, damage_{damage}
    {}

    // методы, доступные всем юнитам ...
    virtual void DecreaseHealth(unsigned int damage) = 0;
    virtual bool IsHillable() const = 0;

    virtual void IncreaseHealth(unsigned int additional_health) {};  // не все должны хиляться

    virtual unsigned int GetHealth() const { return health_; };
protected:
    unsigned int health_;
    unsigned int protection_;
    unsigned int damage_;
};

#endif
