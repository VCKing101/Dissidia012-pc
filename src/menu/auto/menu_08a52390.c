#include "menu/auto_region2.h"

extern unsigned char D_08A52C98[];

/* Widget52C98 constructor. */
Widget52C98 *func_08A52390(Widget52C98 *self)
{
    func_08A52300(&self->base);
    self->base.vtable = D_08A52C98;
    func_08993560(self->field_74, 2, sizeof(Elem5C390), func_08A5C390);
    func_08A523E4(self);
    return self;
}
