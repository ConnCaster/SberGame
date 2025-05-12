#ifndef WAGENBURG_H
#define WAGENBURG_H
#include <interfaces/IUnit.h>

class Wagenburg {
public:
    Wagenburg();
    ~Wagenburg() = default;

    Wagenburg(const Wagenburg& other)
       : health_(other.health_),
         protection_(other.protection_),
         damage_(other.damage_)
    {}

    void GetShoot(unsigned int damage);

    unsigned int GetHealth() const { return health_; }

private:
    unsigned int health_;
    unsigned int protection_;
    unsigned int damage_;
};

// @brief Pattern Adapter
class WagenburgAdapter : public IUnit {
public:
    WagenburgAdapter(Wagenburg* wagenburg)
        : IUnit(wagenburg->GetHealth(), 0, 0), wagenburg_(wagenburg)
    {}

    WagenburgAdapter(const WagenburgAdapter& other)
        : IUnit(other), wagenburg_(new Wagenburg(*other.wagenburg_))
    {}

    IUnit* DeepCopy() {
        return new WagenburgAdapter(*this);
    }

    ~WagenburgAdapter() {
        delete wagenburg_;
    };

    void DecreaseHealth(unsigned int damage) {
        wagenburg_->GetShoot(damage);
    }

    unsigned int GetHealth() const override { return wagenburg_->GetHealth(); }

private:
    Wagenburg* wagenburg_;
};

#endif //WAGENBURG_H
