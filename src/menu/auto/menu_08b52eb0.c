#include "menu/auto_region4.h"

extern unsigned char D_08B534E8[];

/* Widget52EB0 constructor. */
Widget52EB0 *func_08B52EB0(Widget52EB0 *self)
{
    func_0892ABC0(self);
    self->vtable = D_08B534E8;
    func_08B52F54(self);
    return self;
}
