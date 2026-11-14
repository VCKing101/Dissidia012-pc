#include "battle/agent27_types.h"
extern int func_08B1A278(ResHolder *holder);
int func_08B1A758(LoadSet *set)
{
    if (set->active == 0) {
        return 0;
    }
    if (set->state != 2) {
        return 0;
    }
    return func_08B1A278(&set->holder);
}
