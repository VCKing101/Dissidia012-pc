#include "battle/agent27_types.h"

extern int func_08B1A178(ResHolder *holder);
extern int func_08B1987C(LoadJob *job);
extern unsigned char func_08B1A21C(ResHolder *holder);

int func_08B1A570(LoadSet *set)
{
    if (set->active == 0) {
        return -1;
    }
    func_08B1A178(&set->holder);
    {
        int i;
        for (i = 0; i < 9; i++) {
            func_08B1987C(&set->jobs[i]);
        }
    }
    if (set->state == 3 && func_08B1A21C(&set->holder) == 1) {
        set->state = 0;
    }
    return 0;
}
