#include "battle/agent27_types.h"

extern int func_08B291D8(EffectSlot *slot);

int func_08B291E0(EffectSlot *slot)
{
    if (func_08B291D8(slot) == 0) {
        return 0;
    }
    return slot->res != 0;
}
