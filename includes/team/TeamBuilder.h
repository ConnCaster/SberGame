#ifndef SBERGAME_TEAMBUILDER_H
#define SBERGAME_TEAMBUILDER_H

#include "team/Team.h"

// @brief Pattern Builder
class ITeamBuilder {
public:
    virtual ~ITeamBuilder() = default;
    virtual void GenerateTeam() = 0;

    virtual Team* GetTeam() = 0;
};

class TeamBuilderRandom : public ITeamBuilder {
public:
    TeamBuilderRandom(const std::string team_name, unsigned int team_cost_max)
            : team_{new Team{team_name}}, team_cost_{0}, team_cost_max{team_cost_max} {}
    ~TeamBuilderRandom() = default;

    void GenerateTeam() override;
    Team* GetTeam() override;

private:
    Team* team_;
    unsigned int team_cost_;
    unsigned int team_cost_max;
};

class TeamBuilderGreedy : public ITeamBuilder {
public:
    TeamBuilderGreedy(const std::string team_name, unsigned int team_cost_max)
            : team_{new Team{team_name}}, team_cost_{0}, team_cost_max{team_cost_max} {}
    ~TeamBuilderGreedy() = default;

    void GenerateTeam() override;
    Team* GetTeam() override;

private:
    Team* team_;
    unsigned int team_cost_;
    unsigned int team_cost_max;
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

// @brief Pattern FactoryMethod
class ITeamBuilderFactoryM {
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
                // TODO: добавить третий способ генерации команды
                return nullptr;
            }
        }
        // return new TeamBuilderGreedy{team_name, team_max_cost};
        // return new TeamBuilderRandom{team_name, team_max_cost};
    }
};

#endif //SBERGAME_TEAMBUILDER_H
