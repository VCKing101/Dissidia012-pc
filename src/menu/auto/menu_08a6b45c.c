#include "menu/auto_region2.h"

extern unsigned char D_08A6C638[];

/* Panel6C638 deleting destructor. */
void func_08A6B45C(Panel6C638 *self, unsigned int flags)
{
    if (self != 0) {
        self->base.vtable = D_08A6C638;
        func_08A57428(&self->view, 2);
        func_08A51E08(&self->base, 0);
        if ((flags & 1) != 0) {
            func_08992F04(self);
        }
    }
}
