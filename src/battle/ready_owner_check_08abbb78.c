#include "battle/agent15_types.h"

extern int func_08ABAACC(void *part);

int func_08ABBB78(ReadyOwner *self)
{
    if (func_08ABB474(self) == 0) {
        return -1;
    }
    if (self->flag_DA == 1) {
        return 0;
    }
    if (func_08ABAACC(&self->ready_B4) != 0) {
        return -2;
    }
    return 0;
}
