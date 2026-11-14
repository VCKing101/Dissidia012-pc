#include "battle/agent15_types.h"

/* Clear a current flag; -2 for a bit number of 31 or more. */
int func_08AB390C(FlagFrames *self, unsigned int bit)
{
    if (self->active == 0) {
        return -1;
    }
    if (bit >= 31) {
        return -2;
    }
    self->current &= ~(1 << bit);
    return 0;
}
