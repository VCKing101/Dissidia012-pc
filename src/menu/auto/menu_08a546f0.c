#include "menu/auto_region2.h"

extern unsigned char D_08A54E98[];

/* Widget54E98 constructor. */
Widget54E98 *func_08A546F0(Widget54E98 *self)
{
    func_08A52300(&self->base);
    self->base.vtable = D_08A54E98;
    func_08993560(self->field_50, 1, sizeof(Elem5C390), func_08A5C390);
    func_08A54744(self);
    return self;
}
