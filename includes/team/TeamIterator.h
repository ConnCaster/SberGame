#ifndef SBERGAME_TEAMITERATOR_H
#define SBERGAME_TEAMITERATOR_H

#include "interfaces/ITeam.h"

// @brief Pattern Iterator
class TeamIterator {
public:
    TeamIterator(ITeam* team)
            : team_{team}, index_{0}
    {}

    inline bool HasNext() const {
        return index_ < team_->GetSize();
    }

    inline IUnit* Next() {
        return team_->GetUnitByPos(index_++);
    }

private:
    ITeam* team_;
    int index_;
};


#endif //SBERGAME_TEAMITERATOR_H
