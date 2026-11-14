#include "battle/agent14_types.h"

extern "C" int func_08A92B04(HandleTask *self)
{
    if (self->isReady() == 0) {
        return -1;
    }
    return 0;
}
