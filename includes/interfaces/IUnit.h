#ifndef SBERGAME_IUNIT_H
#define SBERGAME_IUNIT_H

class IUnit {
public:
    virtual ~IUnit() = default;
    IUnit(unsigned int health, unsigned int protection, unsigned int damage, bool is_hillable, bool is_clonable)
        : health_{health}, protection_{protection}, damage_{damage}, is_hillable_{is_hillable}, is_clonable_{is_clonable}
    {}

    // реализация паттерна Прототип
    virtual IUnit* Clone() = 0;

    // методы, доступные всем юнитам ...
    virtual void DecreaseHealth(unsigned int damage) = 0;
    virtual void IncreaseHealth(unsigned int additional_health) = 0;  // не все должны хиляться

    virtual unsigned int GetHealth() const { return health_; };
    virtual bool GetIsHillable() const { return is_hillable_; };
    virtual bool GetIsClonable() const { return is_clonable_; };

protected:
    unsigned int health_;
    unsigned int protection_;
    unsigned int damage_;

    bool is_hillable_{false};
    bool is_clonable_{false};
};

#endif
