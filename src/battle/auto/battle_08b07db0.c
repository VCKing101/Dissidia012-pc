#include "battle/auto_region5.h"

/* Part06984: goes to state 0x12 when func_08B0AA58 reports 1. */
int func_08B07DB0(Part06984 *self)
{
    if (func_08B0AA58() == 1) {
        func_08B081D4(self, 0x12);
    }
    return 0;
}
