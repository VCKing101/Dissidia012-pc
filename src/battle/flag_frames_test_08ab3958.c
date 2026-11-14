#include "battle/agent15_types.h"

/* 1 when the flag is set this frame. */
int func_08AB3958(FlagFrames *self, unsigned int bit)
{
    if (self->active == 0) {
        return 0;
    }
    if (bit >= 31) {
        return 0;
    }
    return (self->current & (1 << bit)) != 0;
}
