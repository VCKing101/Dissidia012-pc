#include "battle/agent27_types.h"

extern int func_08B28818(ModelSlot *slot);

int func_08B28820(ModelSlot *slot)
{
    if (func_08B28818(slot) == 0) {
        return 0;
    }
    return slot->list != 0;
}
