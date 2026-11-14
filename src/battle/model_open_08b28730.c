#include "battle/agent27_types.h"

extern void func_08B28650(ModelSlot *slot);

int func_08B28730(ModelSlot *slot, int a, int b, int c)
{
    func_08B28650(slot);
    if (a == 0 || b == 0 || c == 0) {
        return -1;
    }
    slot->field_18 = a;
    slot->field_1C = b;
    slot->field_20 = c;
    slot->active = 1;
    return 0;
}
