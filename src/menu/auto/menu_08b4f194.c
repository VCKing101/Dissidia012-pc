#include "menu/auto_region4.h"

extern unsigned char D_08B4FD50[];

/* Widget4F194 constructor. */
Widget4F194 *func_08B4F194(Widget4F194 *self)
{
    func_0892ABC0(self);
    self->vtable = D_08B4FD50;
    func_08993560(self->field_3C, 4, sizeof(Elem4ECC8), func_08B4ECC8);
    func_08993560(self->field_13C, 2, sizeof(Elem4ECC8), func_08B4ECC8);
    func_08B4F270(self);
    return self;
}
