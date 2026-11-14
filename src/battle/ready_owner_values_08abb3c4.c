#include "battle/agent15_types.h"

int func_08ABB3C4(ReadyOwner *self, float a, float b, float c, float d)
{
    if (func_08ABB474(self) == 0) {
        return -1;
    }
    self->values[0] = a;
    self->values[1] = b;
    self->values[2] = c;
    self->values[3] = d;
    return 0;
}
