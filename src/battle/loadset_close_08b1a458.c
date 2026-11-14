#include "battle/agent27_types.h"

extern void func_08B1966C(LoadJob *job);
extern void func_08B1A0E8(ResHolder *holder);
extern void func_08B1A3F8(LoadSet *set);

void func_08B1A458(LoadSet *set)
{
    {
        int i;
        for (i = 0; i < 9; i++) {
            func_08B1966C(&set->jobs[i]);
        }
    }
    func_08B1A0E8(&set->holder);
    func_08B1A3F8(set);
}
