#include "battle/agent27_types.h"

extern void func_08B1A0D8(ResHolder *holder);
extern void func_08B19628(LoadJob *job);

void func_08B1A3F8(LoadSet *set)
{
    set->active = 0;
    set->state = 0;
    set->field_08 = 0;
    set->count = 0;
    func_08B1A0D8(&set->holder);
    {
        int i;
        for (i = 0; i < 9; i++) {
            func_08B19628(&set->jobs[i]);
        }
    }
}
