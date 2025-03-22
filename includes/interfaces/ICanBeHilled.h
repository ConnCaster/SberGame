#ifndef ICANBEHILLED_H
#define ICANBEHILLED_H
#include "IUnit.h"

class ICanBeHilled {
public:
    virtual ~ICanBeHilled() = default;
    virtual void Hill(unsigned int additional_health) = 0;
};

#endif //ICANBEHILLED_H
