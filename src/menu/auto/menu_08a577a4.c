#include "menu/auto_region2.h"

/* Panel58420: releases its items, then the panel. */
void func_08A577A4(Panel58420 *self)
{
    int i;

    for (i = 0; i < 6; i++) {
        func_08A55044(self->items[i]);
    }
    func_08A51EB8(&self->base);
}
