#include "main/auto_region2.h"

/* Deleting destructor: destroys the members at +0x1AF0, +0x1874 and +0x90. */
void func_0887A19C(Obj87A19C *self, unsigned int flags)
{
    if (self != 0) {
        func_088CE36C(self->field_1AF0, 2);
        func_088D6CFC(self->field_1874, 2);
        func_088DFD94(self->field_90, 2);
        if ((flags & 1) != 0) {
            func_08992F04(self);
        }
    }
}
