#include "Attack.h"

#include "interfaces/IUnit.h"
#include "team/Team.h"

int UnitToUnitAttackMediator::Attack(IUnit* l, IUnit* r) {
    bool was_smbd_killed = false;
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
        std::cout << "HeavyHero [index=" << l_team_->GetHeroNumber(l) << "] attacks... " << ExtractTypeFromUnitPtr(r) << " [index=" << r_team_->GetHeroNumber(r) << "]" << std::endl;
        dynamic_cast<HeavyHero*>(l)->PerformAttack(r);
    } else if ((ExtractTypeFromUnitPtr(l) == "Hero")) {
        std::cout << "Hero [index=" << l_team_->GetHeroNumber(l) << "] attacks... " << ExtractTypeFromUnitPtr(r)<< " [index=" << r_team_->GetHeroNumber(r) << "]" << std::endl;
        dynamic_cast<Hero*>(l)->PerformAttack(r);
    } else if ((ExtractTypeFromUnitPtr(l) == "Archer")) {
        std::cout << "Archer [index=" << l_team_->GetHeroNumber(l) << "] attacks... ";

        if (r_team_->GetFormation() == FormationType::LINE_ALL_ATTACK) {
            std::cout << ExtractTypeFromUnitPtr(r) << " [index=" << r_team_->GetHeroNumber(r) << "]" << std::endl;
            dynamic_cast<Archer*>(l)->PerformAttack(r);
        } else if (r_team_->GetFormation() == FormationType::LINE_FIRST_ONLY) {
            r_team_->ReturnUnit(r);
            IUnit* far_unit = r_team_->GetRandomUnit(dynamic_cast<Archer*>(l)->distance_);
            if (far_unit) {
                std::cout << ExtractTypeFromUnitPtr(r) << " [index=" << r_team_->GetHeroNumber(r) << "]" << std::endl;
                dynamic_cast<Archer*>(l)->PerformAttack(far_unit);
                r = far_unit;
            } else {
                std::cout << "Error getting enemy" << std::endl;
            }
        }

    } else if ((ExtractTypeFromUnitPtr(l) == "Hiller")) {
        std::cout << "Hiller [index=" << l_team_->GetHeroNumber(l) << "] attacks... " << ExtractTypeFromUnitPtr(r) << " [index=" << r_team_->GetHeroNumber(r) << "]" << std::endl;
        dynamic_cast<Hiller*>(l)->PerformAttack(r);
    } else if ((ExtractTypeFromUnitPtr(l) == "Wizard")) {
        std::cout << "Wizard [index=" << l_team_->GetHeroNumber(l) << "] does not have an attack ability" << std::endl;
    } else if ((ExtractTypeFromUnitPtr(l) == "Wagenburg")) {
        std::cout << "Wagenburg [index=" << l_team_->GetHeroNumber(l) << "] does not have an attack ability" << std::endl;
    }
    if (l_team_->GetFormation() == FormationType::LINE_FIRST_ONLY) {
        l_team_->ReturnUnit(l);
    }
    if (r && r->GetHealth() == 0) {
        std::cout << "\tFINISH HIM!!!" << std::endl;
        std::string msg = "[" + r_team_->GetTeamName() + "] " + ExtractTypeFromUnitPtr(r) + " [index=" + std::to_string(r_team_->GetHeroNumber(r)) + "] was killed\n";
        logger_->AddLogMsg(msg, LogType::DEAD);
        // if (r_team_->GetFormation() == FormationType::LINE_FIRST_ONLY) {
            r_team_->RemoveUnit(r);
            delete r;
        // }
        was_smbd_killed = true;
        // return 1;
    } else {
        if (r) {
            if (r_team_->GetFormation() == FormationType::LINE_FIRST_ONLY) {
                r_team_->ReturnUnit(r);
            }
        }
    }
    if (was_smbd_killed) {
        logger_->ExecLog(LogType::DEAD);
        return 1;
    }
    return 0;
}