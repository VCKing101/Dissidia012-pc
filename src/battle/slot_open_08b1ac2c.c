#include "battle/agent27_types.h"

extern void func_08B1AB34(StreamSlot *slot);

int func_08B1AC2C(StreamSlot *slot, int a, int b, int c, int d)
{
    func_08B1AB34(slot);
    if (a == 0 || b == 0 || c == 0 || d == 0) {
        return -1;
    }
    slot->field_2C = a;
    slot->field_30 = b;
    slot->field_34 = c;
    slot->field_38 = d;
    slot->active = 1;
    return 0;
}
