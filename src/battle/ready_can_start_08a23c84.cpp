#include "battle/agent31_types.h"

extern "C" int func_08A235C0(ReadyScreen *self, int arg);
extern "C" int func_08891510(A31Package *package);
extern "C" int func_08891944(A31Package *package);
extern "C" int func_08A07548(void *owner, unsigned int side);
extern "C" int func_0889120C(A31Package *package);
extern "C" int func_08A23AEC(ReadyScreen *self, unsigned int side);
extern "C" int func_08A23BB8(ReadyScreen *self, unsigned int side);

extern "C" bool func_08A23C84(ReadyScreen *self, unsigned int side)
{
    if (side >= 2) {
        return false;
    }
    if (func_08A235C0(self, 0) == 0) {
        return false;
    }
    if (func_08891510(&self->packages[side]) < 0) {
        return false;
    }
    if (func_08891944(&self->packages[side]) == 1) {
        return false;
    }
    if (func_08A07548(self->owner, side) == 1) {
        return false;
    }
    if (func_0889120C(&self->packages[side]) == 0) {
        return false;
    }
    if (func_08A23AEC(self, side) == 1) {
        return false;
    }
    if (func_08A23BB8(self, side) == 1) {
        return false;
    }
    return true;
}
