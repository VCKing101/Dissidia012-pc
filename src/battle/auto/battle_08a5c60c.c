#include "battle/auto_region3.h"

/* Enters state 0 when the motion is 1. */
void func_08A5C60C(void *self, BattleFighter *fighter)
{
    if (fighter->motion == 1) {
        func_08834C10(fighter, 0, 0);
    }
}
