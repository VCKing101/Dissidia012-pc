#include "battle/agent14_types.h"

extern int func_08A85EBC(Marker *self);

int func_08A85F34(Marker *self)
{
    if (self->active == 0) {
        return -1;
    }
    if (func_08A85EBC(self) == 0) {
        return -1;
    }
    return self->id;
}
