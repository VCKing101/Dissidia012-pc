#include "main/auto_region2.h"

/* Deleting destructor: destroys the three element arrays, then the base. */
void func_08879DB0(Obj879DB0 *self, unsigned int flags)
{
    if (self != 0) {
        func_0899372C(self->field_6C0, 6, 0x14, func_08920594, 0, 0);
        func_0899372C(self->field_240, 0x20, 0x24, func_08922898, 0, 0);
        func_0899372C(self->field_10, 0x46, 8, func_08924F68, 0, 0);
        func_089209FC(self, 2);
        if ((flags & 1) != 0) {
            func_08992F04(self);
        }
    }
}
