#include "menu/auto_region2.h"

extern unsigned char D_08A58420[];

/* Panel58420 deleting destructor: destroys its six items. */
void func_08A57428(Panel58420 *self, unsigned int flags)
{
    if (self != 0) {
        self->base.vtable = D_08A58420;
        func_08A56D0C(&self->field_F54, 2);
        func_08A56A8C(&self->field_DFC, 2);
        func_08A55F18(&self->field_AB0, 2);
        func_08A55A4C(&self->field_578, 2);
        func_08A565B8(&self->field_33C, 2);
        func_08A565B8(&self->field_100, 2);
        func_08A51E08(&self->base, 0);
        if ((flags & 1) != 0) {
            func_08992F04(self);
        }
    }
}
