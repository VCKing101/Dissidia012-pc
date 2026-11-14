#include "battle/auto_region3.h"

/* Sets flag 0x20000, calls func_08837A5C, and sets flag 0x1000 in motion
 * 0x1D7. */
void func_08A5B00C(void *self, BattleFighter *fighter)
{
    fighter->flags |= 0x20000;
    func_08837A5C(fighter);
    if (fighter->motion == 0x1D7) {
        fighter->flags |= 0x1000;
    }
}
