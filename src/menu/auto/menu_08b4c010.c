#include "menu/auto_region4.h"

extern unsigned char D_08B4C750[];

/* Widget4C010 constructor. */
Widget4C010 *func_08B4C010(Widget4C010 *self)
{
    func_0892ABC0(self);
    self->vtable = D_08B4C750;
    func_08B4C0B4(self);
    return self;
}
