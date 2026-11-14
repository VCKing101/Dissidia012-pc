#include "menu/auto_region2.h"

extern unsigned char D_08A54438[];

/* Widget54438 constructor. */
Widget54438 *func_08A531E0(Widget54438 *self)
{
    func_08A52300(&self->base);
    self->base.vtable = D_08A54438;
    func_08993560(self->field_E0, 2, sizeof(Elem52D60), func_08A52D60);
    func_08993560(self->field_1F4, 2, sizeof(Elem5C390), func_08A5C390);
    func_08993560(self->field_204, 2, sizeof(Elem5C1E8), func_08A5C1E8);
    func_08993560(self->field_21C, 1, sizeof(Elem5BFF0), func_08A5BFF0);
    func_08A5327C(self);
    return self;
}
