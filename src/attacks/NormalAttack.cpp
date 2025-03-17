#include "attacks/NormalAttack.h"

void NormalAttack::DoAttack(IUnit *target, unsigned int damage) {
    target->DecreaseHealth(damage);
}
