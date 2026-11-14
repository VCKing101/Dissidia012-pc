#include "battle/agent27_types.h"

extern void func_08B1AB34(StreamSlot *slot);
extern void func_08B1C018(ResHolder *holder);
extern void func_08B1C290(StreamSet *set);

void func_08B1C2FC(StreamSet *set)
{
    {
        int i;
        for (i = 0; i < 0x80; i++) {
            func_08B1AB34(&set->slots[i]);
        }
    }
    func_08B1C018(&set->holder);
    func_08B1C290(set);
}
