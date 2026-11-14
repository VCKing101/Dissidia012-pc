#include "battle/agent27_types.h"

extern int func_08B2C558(PairSlot *slot);
extern void func_08B2C6E0(PairSlot *slot);
extern void func_08B2C9D4(PairSlot *slot);
extern int func_08B2CBC0(PairSlot *slot, int a, int b);

int func_08B2C4D8(PairSlot *slot, int a, int b)
{
    if (func_08B2C558(slot) == 0) {
        return -1;
    }
    if (b == 0) {
        return -2;
    }
    func_08B2C6E0(slot);
    func_08B2C9D4(slot);
    func_08B2CBC0(slot, a, b);
    return 0;
}
