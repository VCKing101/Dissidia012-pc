#include "battle/agent27_types.h"

extern void func_08B1C2FC(StreamSet *set);
extern int func_08B1C058(ResHolder *holder, int value);
extern int func_08B1AC2C(StreamSlot *slot, int a, int b, int c, int d);

int func_08B1C35C(StreamSet *set, int state, int a, int b, int c, int d)
{
    func_08B1C2FC(set);
    if (state == 0 || a == 0 || b == 0 || c == 0 || d == 0) {
        return -1;
    }
    func_08B1C058(&set->holder, a);
    {
        int i;
        for (i = 0; i < 0x80; i++) {
            func_08B1AC2C(&set->slots[i], a, b, c, d);
        }
    }
    set->state = state;
    set->active = 1;
    return 0;
}
