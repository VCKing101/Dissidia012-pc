#include "menu/auto_region1.h"

/* Menu root: field_D94 = field_677C[index], sets bit 2 of field_D80. */
void func_08A1F0A8(MenuRoot *self, int index)
{
    self->work->field_D94 = self->work->field_677C[index];
    self->work->field_D80 = self->work->field_D80 | 2;
}
