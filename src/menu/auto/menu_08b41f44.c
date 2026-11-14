#include "menu/auto_region4.h"

/* ScreenB42088: starts its task, which opens ScreenB41CC0 on kind. */
void func_08B41F44(ScreenB42088 *self, int kind)
{
    func_0886503C(&self->task, func_08B41E80, func_08B41EC0, func_08B41EE0);
    func_088650A4(&self->task);
    self->field_38 = kind;
}
