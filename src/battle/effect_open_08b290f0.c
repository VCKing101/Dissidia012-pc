#include "battle/agent27_types.h"

extern void func_08B28F70(EffectSlot *slot);

int func_08B290F0(EffectSlot *slot, int a, int b, int c)
{
    func_08B28F70(slot);
    if (a == 0 || b == 0 || c == 0) {
        return -1;
    }
    slot->field_58 = a;
    slot->field_5C = b;
    slot->field_60 = c;
    slot->active = 1;
    return 0;
}
