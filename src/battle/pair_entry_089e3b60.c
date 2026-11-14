#include "battle/agent1_types.h"

/* Pair entry: claims the first free slot. */
int func_089E3B60(BattlePairEntry *entry, int value, unsigned char a, unsigned char b)
{
    if (!entry->field_00) return -1;
    {
        int i;
        for (i = 0; i < 2; i++) {
            BattlePairSlot *slot = &entry->field_BC[i];
            if (slot->field_00 == 1) continue;
            slot->field_00 = 1;
            slot->field_04 = value;
            slot->field_08 = a;
            slot->field_09 = b;
            goto done;
        }
    }
done:
    return 0;
}
