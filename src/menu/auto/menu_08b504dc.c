#include "menu/auto_region4.h"

extern unsigned char D_08B51400[];

/* Widget504DC constructor. */
Widget504DC *func_08B504DC(Widget504DC *self)
{
    func_0892ABC0(self);
    self->vtable = D_08B51400;
    func_08993560(self->field_38, 5, sizeof(Elem4FE98), func_08B4FE70);
    func_08B50598(self);
    return self;
}
