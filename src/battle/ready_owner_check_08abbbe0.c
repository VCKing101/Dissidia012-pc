#include "battle/agent15_types.h"

extern int func_08ABA1D4(void *part);

int func_08ABBBE0(ReadyOwner *self)
{
    if (func_08ABB474(self) == 0) {
        return -1;
    }
    if (self->flag_DA == 1) {
        return 0;
    }
    if (func_08ABA1D4(self->member_1A4) != 0) {
        return -2;
    }
    return 0;
}
