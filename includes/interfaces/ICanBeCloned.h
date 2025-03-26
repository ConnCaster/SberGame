#ifndef ICANBECLONED_H
#define ICANBECLONED_H

#include "interfaces/IUnit.h"

// @brief Pattern Prototype
class ICanBeCloned {
public:
    virtual ~ICanBeCloned() = default;
    virtual IUnit* Clone() = 0;
};

#endif //ICANBECLONED_H
