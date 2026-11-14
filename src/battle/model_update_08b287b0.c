#include "battle/agent27_types.h"

extern int func_08B28818(ModelSlot *slot);
extern void func_08B2885C(ModelSlot *slot);
extern void func_08B2890C(ModelSlot *slot);
extern void func_08B28A8C(ModelSlot *slot);
extern void func_08B28B94(ModelSlot *slot);
extern int func_08B28D5C(ModelSlot *slot);
extern void func_08B28DCC(ModelSlot *slot);

int func_08B287B0(ModelSlot *slot)
{
    if (func_08B28818(slot) == 0) {
        return -1;
    }
    func_08B2885C(slot);
    func_08B2890C(slot);
    func_08B28A8C(slot);
    func_08B28B94(slot);
    func_08B28D5C(slot);
    func_08B28DCC(slot);
    return 0;
}
