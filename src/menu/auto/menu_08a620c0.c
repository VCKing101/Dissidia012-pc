#include "menu/auto_region2.h"

/* Panel63680: releases its widgets, then the panel. */
void func_08A620C0(Panel63680 *self)
{
    Widget54438 *widget;
    int i;

    func_08A54890(&self->field_E4);
    func_08A52548(&self->field_144);
    i = 0;
    widget = self->field_1D4;
    do {
        func_08A5352C(widget);
        i = i + 1;
        widget = widget + 1;
    } while (i < 5);
    func_08A51EB8(&self->base);
}
