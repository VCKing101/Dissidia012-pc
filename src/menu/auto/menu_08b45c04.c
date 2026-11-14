#include "menu/auto_region4.h"

/* Net45BB8: takes its four values, builds its data parts and starts. */
void func_08B45C04(Net45BB8 *self, int arg1, int arg2, int arg3, int arg4, int arg5)
{
    self->field_00 = arg1;
    self->field_04 = arg2;
    self->field_08 = arg3;
    self->field_0C = arg4;
    func_08B46E7C(self);
    func_08B4A26C(&self->data->field_20);
    func_08B4863C(&self->data->field_24);
    func_08B45A98(&self->data->field_28);
    func_08920A3C(self->data->field_1C, arg5);
    func_08B46F00(self);
    func_08920AC8(self->data->field_1C, self->data->field_04);
}
