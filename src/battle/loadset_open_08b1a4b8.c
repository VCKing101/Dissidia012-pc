#include "battle/agent27_types.h"

extern void func_08B1A458(LoadSet *set);
extern int func_08B1A128(ResHolder *holder, int value);
extern int func_08B197E0(LoadJob *job, unsigned int kind, int a, int b, void *c);

int func_08B1A4B8(LoadSet *set, int a, int b)
{
    func_08B1A458(set);
    if (a == 0 || b == 0) {
        return -1;
    }
    func_08B1A128(&set->holder, a);
    {
        int i;
        for (i = 0; i < 9; i++) {
            func_08B197E0(&set->jobs[i], i, a, b, &set->holder);
        }
    }
    set->active = 1;
    return 0;
}
