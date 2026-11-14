#include "menu/auto_region2.h"

/* Panel694A8: opens in mode with its arguments, state 1. */
void func_08A68494(Panel694A8 *self, int mode, int arg2, int arg3, int arg4, int arg5, int arg6,
                   int arg7, int arg8, int arg9)
{
    int time;

    self->field_28 = mode;
    self->field_D4 = arg2;
    self->field_D8 = arg3;
    self->field_DC = arg4;
    self->field_34 = arg5;
    time = func_088729A0(0);
    self->field_D0 = time;
    func_08A689EC(self);
    func_08A68AA8(self, arg6, arg7, arg8, arg9);
    func_08A68CD8(self);
    func_08A69268(self);
    func_08A68C34(self);
    self->state = 1;
}
