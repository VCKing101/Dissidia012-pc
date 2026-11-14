#include "battle/agent27_types.h"

extern unsigned char func_08B239EC(int handle);

int func_08B29794(EffectSlot *slot)
{
    slot->field_50 = func_08B239EC(slot->field_58);
    return 0;
}
