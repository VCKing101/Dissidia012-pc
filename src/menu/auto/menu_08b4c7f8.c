#include "menu/auto_region4.h"

extern unsigned char D_08B4CA20[];

/* Widget4C7F8 constructor. */
Widget4C7F8 *func_08B4C7F8(Widget4C7F8 *self)
{
    func_0892ABC0(self);
    self->vtable = D_08B4CA20;
    func_08B4C89C(self);
    return self;
}
