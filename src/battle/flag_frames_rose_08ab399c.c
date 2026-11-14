#include "battle/agent15_types.h"

/* 1 when the flag is set this frame but was clear the frame before. */
int func_08AB399C(FlagFrames *self, unsigned int bit)
{
    if (self->active == 0) {
        return 0;
    }
    if (bit >= 31) {
        return 0;
    }
    return (self->current & ~self->previous & (1 << bit)) != 0;
}
