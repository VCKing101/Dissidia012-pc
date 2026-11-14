#include "battle/agent27_types.h"

extern unsigned char func_08B239EC(int handle);

int func_08B28DCC(ModelSlot *slot)
{
    slot->field_0C = func_08B239EC(slot->field_18);
    return 0;
}
