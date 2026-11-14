#include "battle/agent27_types.h"

void func_08B28F18(EffectSlot *slot)
{
    slot->active = 0;
    slot->res = 0;
    {
        int i;
        for (i = 0; i < 9; i++) {
            int j;
            for (j = 0; j < 2; j++) {
                slot->grid[i][j] = 0;
            }
        }
    }
    slot->field_50 = 0;
    slot->field_54 = 0.0f;
    slot->field_58 = 0;
    slot->field_5C = 0;
    slot->field_60 = 0;
}
