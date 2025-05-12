#ifndef SBERGAME_TEAMBUILDER_H
#define SBERGAME_TEAMBUILDER_H

#include "team/Team.h"
#include "interfaces/IUnit.h"

class UnitSelector {
public:
    virtual ~UnitSelector() = default;
    virtual IUnit* SelectHero() = 0;
};

class UnitSelectorByHands : public UnitSelector {
public:
    IUnit* SelectHero() override;
};

// @brief Pattern Builder
class ITeamBuilder {
public:
    virtual ~ITeamBuilder() = default;
    virtual void GenerateTeam() = 0;

    virtual Team* GetTeam() = 0;
};

class TeamBuilderRandom : public ITeamBuilder {
public:
    TeamBuilderRandom(const std::string& team_name, unsigned int team_cost_max)
            : selector_impl_{}, team_{new Team{team_name}}, team_cost_{0}, team_cost_max_{team_cost_max}
    {}
    ~TeamBuilderRandom() override = default;

    void GenerateTeam() override;
    Team* GetTeam() override;

private:
    std::shared_ptr<UnitSelector> selector_impl_;
    Team* team_;
    unsigned int team_cost_;
    unsigned int team_cost_max_;
};

class TeamBuilderGreedy : public ITeamBuilder {
public:
    TeamBuilderGreedy(const std::string& team_name, unsigned int team_cost_max)
            : selector_impl_{}, team_{new Team{team_name}}, team_cost_{0}, team_cost_max_{team_cost_max}
    {}
    ~TeamBuilderGreedy() override = default;

    void GenerateTeam() override;
    Team* GetTeam() override;

private:
    std::shared_ptr<UnitSelector> selector_impl_;
    Team* team_;
    unsigned int team_cost_;
    unsigned int team_cost_max_;
};

class TeamBuilderAllUnits : public ITeamBuilder {
public:
    TeamBuilderAllUnits(const std::string& team_name, unsigned int team_cost_max)
            : selector_impl_{}, team_{new Team{team_name}}, team_cost_{0}, team_cost_max_{team_cost_max}
    {}
    ~TeamBuilderAllUnits() override = default;

    void GenerateTeam() override;
    Team* GetTeam() override;

private:
    std::shared_ptr<UnitSelector> selector_impl_;
    Team* team_;
    unsigned int team_cost_;
    unsigned int team_cost_max_;
    bool all_units_flag = false;
};

class TeamBuilderFromFile : public ITeamBuilder {
public:
    TeamBuilderFromFile(const std::string& team_name, unsigned int team_cost_max)
            : selector_impl_{}, team_{new Team{team_name}}, team_cost_{0}, team_cost_max_{team_cost_max}
    {}
    ~TeamBuilderFromFile() override = default;

    void GenerateTeam() override;
    Team* GetTeam() override;

private:
    std::shared_ptr<UnitSelector> selector_impl_;
    Team* team_;
    unsigned int team_cost_;
    unsigned int team_cost_max_;
    bool enough_money = true;
};

// @brief Pattern Bridge
class TeamBuilderByHandChoose : public ITeamBuilder {
public:
    TeamBuilderByHandChoose(std::shared_ptr<UnitSelectorByHands> selector, const std::string& team_name, unsigned int team_cost_max)
        : selector_impl_{selector}, team_{new Team{team_name}}, team_cost_{0}, team_cost_max_{team_cost_max}
    {}

    ~TeamBuilderByHandChoose() override = default;

    void GenerateTeam() override;
    Team* GetTeam() override;

private:
    std::shared_ptr<UnitSelector> selector_impl_;
    Team* team_;
    unsigned int team_cost_;
    unsigned int team_cost_max_;
};

class Director {
public:
    Director(ITeamBuilder* team_builder)
        : team_builder_(team_builder)
    {}

    void ConstructTeam() {
        team_builder_->GenerateTeam();
    }

private:
    ITeamBuilder* team_builder_;
};

// @brief Pattern Factory
class ITeamBuilderFactory {
public:
    static ITeamBuilder* CreateTeamBuilder(unsigned int type, const std::string team_name, unsigned int team_max_cost) {
        switch (type) {
            case 1: {
                return new TeamBuilderRandom{team_name, team_max_cost};
            }
            case 2: {
                return new TeamBuilderGreedy{team_name, team_max_cost};
            }
            case 3: {
                std::shared_ptr<UnitSelectorByHands> selector_impl = std::make_shared<UnitSelectorByHands>(UnitSelectorByHands());
                return new TeamBuilderByHandChoose{selector_impl, team_name, team_max_cost};
            }
            case 4: {
                return new TeamBuilderAllUnits{team_name, team_max_cost};
            }
            case 5: {
                return new TeamBuilderFromFile{team_name, team_max_cost};
            }
            default:{
                return nullptr;
            }
        }
        // return new TeamBuilderGreedy{team_name, team_max_cost};
        // return new TeamBuilderRandom{team_name, team_max_cost};
        // std::shared_ptr<UnitSelectorByHands> selector_impl = std::make_shared<UnitSelectorByHands>(UnitSelectorByHands());
        // return new TeamBuilderByHandChoose{selector_impl, team_name, team_max_cost};
    }
};

#endif //SBERGAME_TEAMBUILDER_H
