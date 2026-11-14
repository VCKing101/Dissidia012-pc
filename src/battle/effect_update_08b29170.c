#include "battle/agent27_types.h"

extern int func_08B291D8(EffectSlot *slot);
extern void func_08B2921C(EffectSlot *slot);
extern void func_08B292E8(EffectSlot *slot);
extern void func_08B29468(EffectSlot *slot);
extern void func_08B295F4(EffectSlot *slot);
extern void func_08B29670(EffectSlot *slot);
extern void func_08B29794(EffectSlot *slot);

int func_08B29170(EffectSlot *slot)
{
    if (func_08B291D8(slot) == 0) {
        return -1;
    }
    func_08B2921C(slot);
    func_08B292E8(slot);
    func_08B29468(slot);
    func_08B295F4(slot);
    func_08B29670(slot);
    func_08B29794(slot);
    return 0;
}
