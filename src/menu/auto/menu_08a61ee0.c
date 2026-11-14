#include "menu/auto_region2.h"

extern unsigned char D_08A63680[];

/* Panel63680 constructor: builds its widgets. */
Panel63680 *func_08A61EE0(Panel63680 *self)
{
    func_08A51DD0(&self->base);
    self->base.vtable = D_08A63680;
    func_08A546F0(&self->field_E4);
    func_08A52390(&self->field_144);
    func_08993560(self->field_1D4, 5, sizeof(Widget54438), func_08A531E0);
    func_08A61F44(self);
    return self;
}
