#include "battle/auto_region3.h"

/* Enters state 0x18 and clears field_10 of the state data. */
void func_08A573CC(BattleFighter *fighter)
{
    func_08834C10(fighter, 0x18, 0);
    fighter->work->field_10 = 0;
}
