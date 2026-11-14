#include "battle/auto_region3.h"

/* Enters state 0x12 when bit 0 of field_188 is set. */
void func_08A58064(void *self, BattleFighter *fighter)
{
    if (fighter->field_188 & 1) {
        func_08834C10(fighter, 0x12, 0);
    }
}
