#include "battle/agent15_types.h"

/* 1 when the flag was set the frame before. */
int func_08AB39EC(FlagFrames *self, unsigned int bit)
{
    if (self->active == 0) {
        return 0;
    }
    if (bit >= 31) {
        return 0;
    }
    return (self->previous & (1 << bit)) != 0;
}
