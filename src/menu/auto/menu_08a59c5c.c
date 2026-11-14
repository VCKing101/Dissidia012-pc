#include "menu/auto_region2.h"

extern unsigned char D_08A5B608[];

/* Panel5B608 constructor: builds its parts and its Panel60670. */
Panel5B608 *func_08A59C5C(Panel5B608 *self)
{
    func_08A51DD0(&self->base);
    self->base.vtable = D_08A5B608;
    func_08993560(self->field_78, 3, sizeof(Part5B578), func_08A58868);
    func_08993560(self->field_1B0, 5, sizeof(Part5B5C0), func_08A59028);
    func_08A601B0(&self->field_1984);
    func_08A59CD0(self);
    return self;
}
