#include "battle/agent27_types.h"

extern "C" {

void func_088DA060(Resource *res);

struct HolderDrop6 {
    static unsigned char flag20(Resource *res)
    {
        return (res->flags & 0x20) ? 1 : 0;
    }
};

int func_08B1C0A8(ResHolder *self)
{
    if (self->active == 0) {
        return -1;
    }
    Resource *res = self->res;
    if (res != 0 && res->kind == 6 && HolderDrop6::flag20(res) == 1) {
        if (res != 0) {
            func_088DA060(res);
            self->res = 0;
        }
    }
    return 0;
}

}
