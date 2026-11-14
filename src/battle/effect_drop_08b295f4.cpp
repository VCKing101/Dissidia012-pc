#include "battle/agent27_types.h"

extern "C" {

void func_088DA060(Resource *res);
void func_08B28FB4(EffectSlot *slot);

struct EffectDrop {
    static unsigned char flag20(Resource *res)
    {
        return (res->flags & 0x20) ? 1 : 0;
    }
};

int func_08B295F4(EffectSlot *slot)
{
    Resource *res = slot->res;
    if (res != 0 && res->kind == 0x10 && EffectDrop::flag20(res) == 1) {
        func_08B28FB4(slot);
        if (slot->res != 0) {
            func_088DA060(slot->res);
            slot->res = 0;
        }
    }
    return 0;
}

}
