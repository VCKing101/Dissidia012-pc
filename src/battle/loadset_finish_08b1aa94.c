#include "battle/agent27_types.h"

extern void func_08B1A358(ResHolder *holder);

int func_08B1AA94(LoadSet *set)
{
    if (set->active == 0) {
        return -1;
    }
    if (set->state != 2) {
        return -2;
    }
    func_08B1A358(&set->holder);
    set->state = 3;
    return 0;
}
