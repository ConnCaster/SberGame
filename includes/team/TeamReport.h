#ifndef TEAMREPORT_H
#define TEAMREPORT_H

#include "interfaces/ITeam.h"
#include "team/Team.h"

// @brief Pattern TemplateMethod
class ReportGenerator {
public:
    void GenerateReport() {
        ProcessData();
        FormatReport();
    }

    virtual ~ReportGenerator() = default;

protected:
    std::string formatted_report_;

    virtual void ProcessData() = 0;
    virtual void FormatReport() = 0;
};

class TeamReportGenerator : public ReportGenerator {
public:
    TeamReportGenerator(ITeam* team)
        : team_(team)
    {}
    ~TeamReportGenerator() = default;

    void ProcessData() override {
        formatted_report_ += "Team: '" + team_->GetTeamName() + "' of " + std::to_string(team_->GetSize()) + " units\n";
        TeamIterator *iter = dynamic_cast<Team*>(team_)->CreateIterator();
        int i = 0;
        while (iter->HasNext()) {
            IUnit *unit = iter->Next();
            formatted_report_ += "\t[" + std::to_string(i + 1) + "] " + ExtractTypeFromUnitPtr(unit) + " [index=" +
                    std::to_string(team_->GetHeroNumber(unit)) + "]\n\t\t" + unit->GetInfo() + "\n";
            i++;
        }
    }

    void FormatReport() override {
        std::cout << formatted_report_ << std::endl;
    }
private:
    ITeam* team_;
};

#endif //TEAMREPORT_H
