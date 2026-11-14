#include "menu/auto_region2.h"

extern unsigned char D_08A68180[];

/* Panel68180 constructor: builds its pages. */
Panel68180 *func_08A66218(Panel68180 *self)
{
    func_08A51DD0(&self->base);
    self->base.vtable = D_08A68180;
    func_08993560(self->field_188, 2, sizeof(Page67EE0), func_08A63A48);
    func_08A644A4(&self->field_12D0);
    func_08A64B84(&self->field_131C);
    func_08993560(self->field_13B4, 0x14, sizeof(Page680D8), func_08A65420);
    func_08A66294(self);
    return self;
}
