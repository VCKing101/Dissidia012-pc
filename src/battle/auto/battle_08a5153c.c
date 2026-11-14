#include "battle/auto_region3.h"

/* Stops the fighter (func_08A519D4) and clears flag 0x200. */
void func_08A5153C(void *self, BattleFighter *fighter)
{
    func_08A519D4(self, fighter);
    fighter->flags &= ~0x200;
}
