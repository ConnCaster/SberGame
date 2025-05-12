#ifndef SBERGAME_HEAVYHERO_H
#define SBERGAME_HEAVYHERO_H

#include <memory>
#include <iostream>

#include "attacks/NormalAttack.h"
#include "interfaces/IAttack.h"
#include "interfaces/IUnit.h"

class HeavyHero : public IUnit {
public:
    HeavyHero();
    virtual ~HeavyHero() override = default;

    HeavyHero(const HeavyHero& other)
        : IUnit(other),  // копируем базовый IUnit
          attack_(std::make_unique<NormalAttack>())
    {}

    IUnit* DeepCopy() {
        return new HeavyHero(*this);
    }

    void DecreaseHealth(unsigned int damage) override;

    // атака и спецвозможности ...
    void SetAttack(std::unique_ptr<IAttack> attack);
    virtual void PerformAttack(IUnit *target);

    // методы работы с усилителями HeavyHero (которые на телеге едут)
    virtual bool HasBuff(const std::string &buff_name) const { return  false; }
    virtual HeavyHero* RemoveBuff(const std::string &buff_name) { return nullptr; }

protected:
    std::unique_ptr<IAttack> attack_;
};

std::string ExtractHeavyHeroTypeFromUnitPtr(HeavyHero* unit);

// @brief Pattern Decorator
class HeavyHeroDecorator : public HeavyHero {
protected:
    HeavyHero* inner_heavy_hero_;
    std::string buff_name_;
    int attack_bonus_;
    int defense_bonus_;

public:
    HeavyHeroDecorator(HeavyHero* hero, const std::string &buff_name, int attack, int defense)
        : inner_heavy_hero_(hero), buff_name_(buff_name), attack_bonus_(attack), defense_bonus_(defense) {
    }

    HeavyHeroDecorator(const HeavyHeroDecorator& other)
            : HeavyHero(other),
              buff_name_(other.buff_name_),
              attack_bonus_(other.attack_bonus_),
              defense_bonus_(other.defense_bonus_),
              inner_heavy_hero_(other.inner_heavy_hero_ ? dynamic_cast<HeavyHero *>(other.inner_heavy_hero_->DeepCopy()) : nullptr)  // рекурсивное копирование
        {}

    IUnit* DeepCopy() {
        return new HeavyHeroDecorator(*this);
    }

    virtual ~HeavyHeroDecorator() = default;

    void DecreaseHealth(unsigned int damage) override {
        std::cout << "[HeavyHero] Defense buff " << buff_name_ << std::endl;
        inner_heavy_hero_->DecreaseHealth(damage - defense_bonus_);
    }

    bool HasBuff(const std::string &buff_name) const override {
        if (buff_name == buff_name_) {
            return true;
        }
        return inner_heavy_hero_->HasBuff(buff_name);
    }

    HeavyHero* RemoveBuff(const std::string &buff_name) override {
        if (buff_name_ == "Spear") attack_bonus_ = 0;
        if (buff_name == buff_name_) {
            HeavyHero* tmp = inner_heavy_hero_;
            inner_heavy_hero_ = nullptr;
            return tmp;
        } else {
            inner_heavy_hero_->RemoveBuff(buff_name);
            return inner_heavy_hero_;
        }
    }

    virtual void PerformAttack(IUnit *target) {
        inner_heavy_hero_->PerformAttack(target);
    }

    HeavyHero* GetInnerHeavyHeroDecorator() {
        return inner_heavy_hero_;
    }

    HeavyHero* GetInnerHeavyHeroOrigin() {
        HeavyHero* inner_unit{nullptr};
        HeavyHero* tmp_unit{inner_heavy_hero_};
        if (ExtractHeavyHeroTypeFromUnitPtr(tmp_unit) == "HeavyHero") {
            return tmp_unit;
        }
        while (dynamic_cast<HeavyHeroDecorator*>(tmp_unit)->GetInnerHeavyHeroDecorator()) {
            inner_unit = dynamic_cast<HeavyHeroDecorator*>(tmp_unit)->GetInnerHeavyHeroDecorator();
            tmp_unit = inner_unit;
            if (ExtractHeavyHeroTypeFromUnitPtr(inner_unit) == "HeavyHero") break;
        }
        return inner_unit;
    }
};


class HorseDecorator : public HeavyHeroDecorator {
public:
    HorseDecorator(HeavyHero* hero)
        : HeavyHeroDecorator(hero, "Horse", 0, 5) {}

    HorseDecorator(const HorseDecorator& other)
        : HeavyHeroDecorator(other)  // делегируем базовому HeavyHeroDecorator
    {}

    IUnit* DeepCopy() {
        return new HorseDecorator(*this);
    }

    virtual ~HorseDecorator() = default;
};

class SpearDecorator : public HeavyHeroDecorator {
public:
    SpearDecorator(HeavyHero* hero)
        : HeavyHeroDecorator(hero, "Spear", 10, 0) {}

    SpearDecorator(const HorseDecorator& other)
            : HeavyHeroDecorator(other)  // делегируем базовому HeavyHeroDecorator
        {}

    IUnit* DeepCopy() {
        return new SpearDecorator(*this);
    }

    void PerformAttack(IUnit *target) {
        std::cout << "[HeavyHero] Attack buff " << buff_name_ << std::endl;
        if (attack_) {
            attack_->DoAttack(target, damage_ + attack_bonus_);
        } else {
            std::cout << "[HeavyHero] There is no attack set. Abort..." << std::endl;
        }
    }

    virtual ~SpearDecorator() = default;
};

class ShieldDecorator : public HeavyHeroDecorator {
public:
    ShieldDecorator(HeavyHero* hero)
        : HeavyHeroDecorator(hero, "Shield", 0, 8) {}

    ShieldDecorator(const HorseDecorator& other)
                : HeavyHeroDecorator(other)  // делегируем базовому HeavyHeroDecorator
            {}

    IUnit* DeepCopy() {
        return new ShieldDecorator(*this);
    }

    virtual ~ShieldDecorator() = default;
};

class HelmetDecorator : public HeavyHeroDecorator {
public:
    HelmetDecorator(HeavyHero* hero)
        : HeavyHeroDecorator(hero, "Helmet", 0, 3) {}

    HelmetDecorator(const HorseDecorator& other)
                : HeavyHeroDecorator(other)  // делегируем базовому HeavyHeroDecorator
            {}

    IUnit* DeepCopy() {
        return new HelmetDecorator(*this);
    }

    virtual ~HelmetDecorator() = default;
};

HeavyHero* AppendBuffToHeavyHero(HeavyHero* heavy_hero);

#endif //SBERGAME_HEAVYHERO_H
