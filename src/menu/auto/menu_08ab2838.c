#include "menu/auto_region3.h"

/* PanelAB2D24: sets up, clears field_568, enters state 2; returns 0. */
int func_08AB2838(PanelAB2D24 *self)
{
    func_08AB2C5C(self);
    self->work->field_568 = 0;
    self->work->state = 2;
    return 0;
}
