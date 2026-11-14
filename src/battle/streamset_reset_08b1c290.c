#include "battle/agent27_types.h"

extern void func_08B1C008(ResHolder *holder);
extern void func_08B1AAF8(StreamSlot *slot);

void func_08B1C290(StreamSet *set)
{
    set->active = 0;
    set->state = 0;
    set->phase = 0;
    set->field_0C = 0.0f;
    set->field_10 = 0;
    set->count = 0;
    func_08B1C008(&set->holder);
    {
        int i;
        for (i = 0; i < 0x80; i++) {
            func_08B1AAF8(&set->slots[i]);
        }
    }
}
