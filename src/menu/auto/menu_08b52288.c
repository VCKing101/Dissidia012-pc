#include "menu/auto_region4.h"

extern unsigned char D_08B52550[];

/* Widget52288 constructor. */
Widget52288 *func_08B52288(Widget52288 *self)
{
    func_0892ABC0(self);
    self->vtable = D_08B52550;
    func_08B5232C(self);
    return self;
}
