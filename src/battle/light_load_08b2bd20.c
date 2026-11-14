#include "battle/agent27_types.h"

extern int func_08B2BD98(ModelSlot *slot);
extern void func_08B2BEDC(ModelSlot *slot);
extern int func_08B2C120(ModelSlot *slot, int a, int b);

int func_08B2BD20(ModelSlot *slot, int a, int b)
{
    if (func_08B2BD98(slot) == 0) {
        return -1;
    }
    if (b == 0) {
        return -2;
    }
    func_08B2BEDC(slot);
    func_08B2C120(slot, a, b);
    return 0;
}
