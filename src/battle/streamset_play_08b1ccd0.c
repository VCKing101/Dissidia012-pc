#include "battle/agent27_types.h"

extern void func_08B1C18C(ResHolder *holder);

int func_08B1CCD0(StreamSet *set)
{
    if (set->active == 0) {
        return -1;
    }
    if (set->phase != 1) {
        return -2;
    }
    if (set->field_10 > 0) {
        func_08B1C18C(&set->holder);
        set->phase = 2;
    } else {
        set->phase = 0;
    }
    return 0;
}
