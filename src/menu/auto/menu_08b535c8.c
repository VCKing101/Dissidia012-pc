#include "menu/auto_region4.h"

extern unsigned char D_08B53B10[];

/* Widget535C8 constructor. */
Widget535C8 *func_08B535C8(Widget535C8 *self)
{
    func_0892ABC0(self);
    self->vtable = D_08B53B10;
    func_08B53604(self);
    return self;
}
