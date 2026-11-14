#include "menu/auto_region4.h"

extern unsigned char D_08B52E10[];

/* Widget525B0 constructor. */
Widget525B0 *func_08B525B0(Widget525B0 *self)
{
    func_0892ABC0(self);
    self->vtable = D_08B52E10;
    func_08B52654(self);
    return self;
}
