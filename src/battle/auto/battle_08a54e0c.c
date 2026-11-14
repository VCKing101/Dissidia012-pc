#include "battle/auto_region3.h"

/* Unless func_0883752C reports otherwise, sets motions 0x65/0x66 and calls
 * func_08834948. */
void func_08A54E0C(void *self, BattleFighter *fighter)
{
    if (func_0883752C(fighter) == 0) {
        func_08834934(fighter, 0x65, 0x66);
        func_08834948(fighter);
    }
}
