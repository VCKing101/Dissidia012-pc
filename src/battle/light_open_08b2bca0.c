#include "battle/agent27_types.h"

extern void func_08B2BBC0(ModelSlot *slot);

int func_08B2BCA0(ModelSlot *slot, int a, int b, int c)
{
    func_08B2BBC0(slot);
    if (a == 0 || b == 0 || c == 0) {
        return -1;
    }
    slot->field_14 = a;
    slot->field_18 = b;
    slot->field_1C = c;
    slot->active = 1;
    return 0;
}
