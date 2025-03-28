#include "attacks/DistanceAttack.h"

void DistanceAttack::DoAttack(IUnit *target, unsigned int damage) {
    target->DecreaseHealth(damage);
}
