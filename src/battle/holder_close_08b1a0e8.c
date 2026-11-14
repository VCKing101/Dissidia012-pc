#include "battle/agent27_types.h"

extern void func_088DA060(Resource *res);
extern void func_08B1A0D8(ResHolder *self);

void func_08B1A0E8(ResHolder *self)
{
    if (self->res != 0) {
        func_088DA060(self->res);
        self->res = 0;
    }
    func_08B1A0D8(self);
}
