#include "battle/agent15_types.h"

extern int func_08ABDAB8(void *member, int arg);

int func_08ABB798(ReadyOwner *self, int arg)
{
    if (func_08ABB474(self) == 0) {
        return -1;
    }
    if (self->flag_DA == 1) {
        return 0;
    }
    return func_08ABDAB8(self->member_15C, arg);
}
