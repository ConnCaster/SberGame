#include "Attack.h"

#include "interfaces/IUnit.h"
#include "team/Team.h"

int AttackFacade::Attack(IUnit* l, IUnit* r) {
    // сбиваем защиту HeavyHero с вероятностью 50%
    if (ExtractTypeFromUnitPtr(r) == "HeavyHero") {
        std::string buff_type = ExtractHeavyHeroTypeFromUnitPtr(dynamic_cast<HeavyHero*>(r));
        if (buff_type == "HorseDecorator" ||
            buff_type == "SpearDecorator" ||
            buff_type == "ShieldDecorator" ||
            buff_type == "HelmetDecorator"
                ) {
            if ((rand() % 100) < 50) {
                std::cout << "[HeavyHero] Buff " << buff_type << " is losed" << std::endl;
                r = dynamic_cast<HeavyHeroDecorator*>(r)->RemoveBuff(buff_type);
            } else {
                std::cout << "[HeavyHero] Buff " << buff_type << " is saved" << std::endl;
            }
        }
    }
    std::cout << "[" << l_team_->GetTeamName() << "] ";
    if (ExtractTypeFromUnitPtr(l) == "HeavyHero") {
        std::cout << "HeavyHero attacks... " << ExtractTypeFromUnitPtr(r) << std::endl;
        dynamic_cast<HeavyHero*>(l)->PerformAttack(r);
    } else if ((ExtractTypeFromUnitPtr(l) == "Hero")) {
        std::cout << "Hero attacks... " << ExtractTypeFromUnitPtr(r) << std::endl;
        dynamic_cast<Hero*>(l)->PerformAttack(r);
    } else if ((ExtractTypeFromUnitPtr(l) == "Archer")) {
        std::cout << "Archer attacks... " << ExtractTypeFromUnitPtr(r) << std::endl;

        r_team_->ReturnUnit(r);
        IUnit* far_unit = r_team_->GetRandomUnit(dynamic_cast<Archer*>(l)->distance_);
        if (far_unit) {
            dynamic_cast<Archer*>(l)->PerformAttack(far_unit);
            r = far_unit;
        }

    } else if ((ExtractTypeFromUnitPtr(l) == "Hiller")) {
        std::cout << "Hiller attacks... " << ExtractTypeFromUnitPtr(r) << std::endl;
        dynamic_cast<Hiller*>(l)->PerformAttack(r);
    } else if ((ExtractTypeFromUnitPtr(l) == "Wizard")) {
        std::cout << "Wizard does not have an attack ability" << std::endl;
    } else if ((ExtractTypeFromUnitPtr(l) == "Wagenburg")) {
        std::cout << "Wagenburg does not have an attack ability" << std::endl;
    }
    l_team_->ReturnUnit(l);
    if (r && r->GetHealth() == 0) {
        std::cout << "\tFINISH HIM!!!" << std::endl;
        std::string msg = "[" + r_team_->GetTeamName() + "] " + ExtractTypeFromUnitPtr(r) + " was killed\n";
        logger_->Log(msg);
        delete r;
        return 1;
    } else {
        if (r) {
            r_team_->ReturnUnit(r);
        }
    }
    return 0;
}