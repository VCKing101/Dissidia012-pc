#include "battle/auto_region3.h"

/* Sets motions 0x65/0x66 and clears flag 0x1000. */
void func_08A5C5CC(void *self, BattleFighter *fighter)
{
    func_08834934(fighter, 0x65, 0x66);
    fighter->flags &= ~0x1000;
}
