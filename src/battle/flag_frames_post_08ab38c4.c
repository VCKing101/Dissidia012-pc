#include "battle/agent15_types.h"

/* Set a pending flag for the next frame; -2 for a bit number of 31 or more. */
int func_08AB38C4(FlagFrames *self, unsigned int bit)
{
    if (self->active == 0) {
        return -1;
    }
    if (bit >= 31) {
        return -2;
    }
    self->pending |= 1 << bit;
    return 0;
}
