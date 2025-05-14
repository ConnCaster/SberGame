#include <team/Team.h>

Team::Team(const std::string& team_name)
    : units_{}, team_name_(team_name), number_manager_(new UnitNumberManager)
{}

Team::~Team() {
    // TODO: раскомментировать
    for (auto unit: units_) {
        delete unit;
    }
}

Team::Team(const Team& other)
    : team_name_(other.team_name_),
        number_manager_(other.number_manager_->DeepCopy()),
        current_row_(other.current_row_),
        current_unit_in_row_(other.current_unit_in_row_),
        formation_(other.formation_)
{
    for (const auto& unit : other.units_) {
        IUnit* new_unit = unit->DeepCopy();
        units_.push_back(new_unit);
        number_manager_->ReplaceHero(unit, new_unit);
    }
}


TeamIterator* Team::CreateIterator() {
    return new TeamIterator(this);
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
    units_.push_front(unit);
}

void Team::ReplaceUnit(IUnit *unit, unsigned int pos) {
    if (pos < 0 || pos >= GetSize()) return;
    IUnit* tmp_unit{unit};
    if (dynamic_cast<HeavyHeroDecorator*>(tmp_unit) != nullptr) {
        tmp_unit = dynamic_cast<HeavyHeroDecorator*>(tmp_unit)->GetInnerHeavyHeroOrigin();
    }
    number_manager_->ReplaceHero(tmp_unit, unit);
    units_.at(pos) = unit;
}

void Team::AddUnit(IUnit* unit) {
    if (!number_manager_->HasNumber(unit)) {
        number_manager_->AssignNumber(unit);
    }
    units_.push_back(unit);
}

void Team::AddUnitToPos(IUnit* unit, unsigned int pos) {
    if (pos >= units_.size()) {
        units_.resize(pos + 1, nullptr);
    }
    if (!number_manager_->HasNumber(unit)) {
        number_manager_->AssignNumber(unit);
    }
    units_.insert(units_.begin() + pos, unit);
}

unsigned int Team::GetHeroNumber(IUnit* unit) const {
    return number_manager_->GetNumber(unit);
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

std::string Team::GetTeamInfo() const {
    std::string out{"Team: '" + team_name_ + "' of " + std::to_string(GetSize()) + " units\n"};
    for (int i = 0; i < units_.size(); ++i) {
        out += "\t[" + std::to_string(i+1) + "] " + ExtractTypeFromUnitPtr(units_[i]) + " [index=" + std::to_string(GetHeroNumber(units_[i])) + "]\n\t\t" + units_[i]->GetInfo() + "\n";
    }
    return out;
}

IUnit* Team::GetUnitByPos(unsigned int pos) {
    if (units_.empty()) return nullptr;
    // берем от начала команды
    std::deque<IUnit*>::iterator it = units_.begin() + pos;
    return *it;
}


std::string ExtractTypeFromUnitPtr(IUnit* unit) {
    if (unit == nullptr) return "";
    if (typeid(*unit) == typeid(HeavyHero)) return "HeavyHero";
    if (typeid(*unit) == typeid(HorseDecorator)) return "HeavyHero";
    if (typeid(*unit) == typeid(SpearDecorator)) return "HeavyHero";
    if (typeid(*unit) == typeid(ShieldDecorator)) return "HeavyHero";
    if (typeid(*unit) == typeid(HelmetDecorator)) return "HeavyHero";
    if (typeid(*unit) == typeid(Hero)) return "Hero";
    if (typeid(*unit) == typeid(Archer)) return "Archer";
    if (typeid(*unit) == typeid(Hiller)) return "Hiller";
    if (typeid(*unit) == typeid(Wizard)) return "Wizard";
    if (typeid(*unit) == typeid(WagenburgAdapter)) return "Wagenburg";
    return "";
}

std::string ExtractHeavyHeroTypeFromUnitPtr(HeavyHero* unit) {
    if (unit == nullptr) return "";
    if (typeid(*unit) == typeid(HeavyHero)) return "HeavyHero";
    if (typeid(*unit) == typeid(HorseDecorator)) return "HorseDecorator";
    if (typeid(*unit) == typeid(SpearDecorator)) return "SpearDecorator";
    if (typeid(*unit) == typeid(ShieldDecorator)) return "ShieldDecorator";
    if (typeid(*unit) == typeid(HelmetDecorator)) return "HelmetDecorator";
    return "";
}

std::vector<std::pair<IUnit*, unsigned int>> Team::CheckIfHeavyHeroNeighbour(unsigned int pos) const {
    // если в команде один герой - у него нет соседа
    if (GetSize() == 1 || GetSize() == 0) {
        return {{nullptr, 0}};
    }
    std::vector<std::pair<IUnit*, unsigned int>> res{};
    // если позиция последняя в команде, то проверяем только предыдущего героя - не HeavyHero?
    if (pos == (GetSize() - 1)) {
        if (ExtractTypeFromUnitPtr(units_.at(pos - 1)) == "HeavyHero") {
            res.emplace_back(units_.at(pos - 1), pos-1);
            return res;
        } else {
            res.emplace_back(nullptr, 0);
            return res;
        }
    }
    // если позиция в середине команды, проверяем соседа слева и справа
    if (ExtractTypeFromUnitPtr(units_.at(pos - 1)) == "HeavyHero" && ExtractTypeFromUnitPtr(units_.at(pos + 1)) == "HeavyHero") {
        res.emplace_back(units_.at(pos - 1), pos - 1);
        res.emplace_back(units_.at(pos + 1), pos + 1);
        return res;
    } else if (ExtractTypeFromUnitPtr(units_.at(pos + 1)) == "HeavyHero") {
        res.emplace_back(units_.at(pos + 1), pos + 1);
        return res;
    } else if (ExtractTypeFromUnitPtr(units_.at(pos - 1)) == "HeavyHero") {
        res.emplace_back(units_.at(pos - 1), pos - 1);
        return res;
    }
    return {};
}

unsigned int Team::GenPosAroundUnit(int pos, int distance) const {
    // Определяем границы
    int left_bound = std::max(0, pos - distance);
    int right_bound = std::min(static_cast<int>(units_.size()) - 1, pos + distance);

    // Собираем все возможные позиции, кроме текущей (pos)
    std::vector<int> possible_positions;
    for (int p = left_bound; p <= right_bound; ++p) {
        if (p != pos) {
            possible_positions.push_back(p);
        }
    }

    // Если нет доступных позиций, возвращаем исходную
    if (possible_positions.empty()) {
        return pos;
    }

    // Выбираем случайную позицию из возможных
    int random_index = rand() % possible_positions.size();
    return possible_positions[random_index];
}
