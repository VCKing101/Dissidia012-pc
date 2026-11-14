#include "battle/agent27_types.h"

int func_08B1AEAC(StreamSlot *slot)
{
    if (slot->active == 0) {
        return 0;
    }
    if (slot->state != 2) {
        return 0;
    }
    if (slot->res == 0) {
        return 0;
    }
    return slot->res->kind == 9;
}
