#include "battle/auto_region4.h"

/* Whether the fighter is set and in motion 0x32D. */
unsigned char func_08AB421C(void *self, BattleFighter *fighter)
{
    if (fighter == 0) {
        return 0;
    } else {
        return fighter->motion == 0x32D;
    }
}
