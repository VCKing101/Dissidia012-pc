#include "battle/agent27_types.h"

void func_08B2C2F8(PairSlot *slot)
{
    slot->active = 0;
    slot->field_04 = 0;
    {
        int i;
        for (i = 0; i < 2; i++) {
            slot->handles[i] = 0;
        }
    }
    slot->field_10 = 0;
    slot->field_14 = 0;
    slot->field_18 = 0;
    slot->field_1C = 0;
    slot->field_20 = 0;
}
