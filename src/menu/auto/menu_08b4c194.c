#include "menu/auto_region4.h"

void func_08B4C194(Widget4C010 *self, int arg1, int arg2, int arg3, int arg4, int arg5)
{
    self->field_48 = arg5;
    func_08B4C500(self);
    func_08B4C5D4(self, arg1, arg2, arg3, arg4);
    self->state = 1;
}
