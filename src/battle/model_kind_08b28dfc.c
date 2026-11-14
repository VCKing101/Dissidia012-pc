#include "battle/agent27_types.h"

extern int D_08B28E5C[];
extern unsigned int func_08B23D4C(int handle);

int func_08B28DFC(ModelSlot *slot)
{
    unsigned int kind = func_08B23D4C(slot->field_18);

    if (kind >= 10) {
        return -1;
    }
    return D_08B28E5C[kind];
}
