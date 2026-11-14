#include "battle/agent15_types.h"

/* Start a new frame: current becomes previous, pending becomes current. */
int func_08AB3848(FlagFrames *self)
{
    if (self->active == 0) {
        return -1;
    }
    self->previous = self->current;
    self->current = self->pending;
    self->pending = 0;
    return 0;
}
