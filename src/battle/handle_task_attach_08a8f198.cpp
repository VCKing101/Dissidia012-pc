#include "battle/agent14_types.h"

extern "C" {

extern int func_08A87AA4(int state, HandleTask *task);

int func_08A8F198(HandleTask *self)
{
    if (self->isReady() == 0) {
        return -1;
    }
    if (func_08A87AA4(self->handle, self) != 0) {
        return -3;
    }
    return 0;
}

}
