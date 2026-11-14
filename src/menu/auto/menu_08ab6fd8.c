#include "menu/auto_region3.h"

/* ObjB41E0: sets flag 0x200; with field_30 bit 1, opens a dialog (func_08AB43D4 on close). */
void func_08AB6FD8(ObjB41E0 *self)
{
    self->flags = self->flags | 0x200;
    if ((self->field_30 & 1) != 0) {
        self->field_30 = self->field_30 | 0x10;
        func_088B1354(D_089D727C, 0, func_08AB43D4, 0, 0xA0);
    }
}
