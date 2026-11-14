#include "menu/auto_region2.h"

extern unsigned char D_08A5DB80[];

/* Panel5DB80 constructor: builds its Panel60670 and sixteen cells. */
Panel5DB80 *func_08A5CBA4(Panel5DB80 *self)
{
    func_08A51DD0(&self->base);
    self->base.vtable = D_08A5DB80;
    func_08A601B0(&self->field_50);
    func_08993560(self->cells, 16, sizeof(Cell5C5C8), func_08A5C5C8);
    func_08A5CC00(self);
    return self;
}
