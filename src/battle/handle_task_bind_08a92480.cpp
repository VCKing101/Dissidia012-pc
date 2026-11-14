#include "battle/agent14_types.h"

extern "C" int func_08A92480(HandleTask *self, int handle)
{
    self->reset();
    if (handle == 0) {
        return -1;
    }
    self->handle = handle;
    self->bound = 1;
    return 0;
}
