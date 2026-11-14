#include "battle/agent27_types.h"

int func_08B1A240(ResHolder *self)
{
    if (self->active == 0) {
        return 0;
    }
    if (self->res == 0) {
        return 0;
    }
    return self->res->kind == 0;
}
