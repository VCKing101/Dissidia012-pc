#include "menu/auto_region2.h"

extern unsigned char D_08A58420[];

/* Panel58420 constructor: builds its six items. */
Panel58420 *func_08A573B4(Panel58420 *self)
{
    func_08A51DD0(&self->base);
    self->base.vtable = D_08A58420;
    func_08A5657C(&self->field_100);
    func_08A5657C(&self->field_33C);
    func_08A55A10(&self->field_578);
    func_08A55EDC(&self->field_AB0);
    func_08A56A50(&self->field_DFC);
    func_08A56CD0(&self->field_F54);
    func_08A574CC(self);
    func_08A575A4(self);
    return self;
}
