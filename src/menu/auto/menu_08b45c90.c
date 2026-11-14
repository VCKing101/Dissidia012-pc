#include "menu/auto_region4.h"

void func_08B45C90(Net45BB8 *self, int arg1, int arg2, int arg3, int arg4, int arg5)
{
    self->field_00 = arg1;
    self->field_04 = arg2;
    self->field_08 = arg3;
    self->field_0C = arg4;
    func_08B46E7C(self);
    func_08B4A2F4(&self->data->field_20);
    func_08B4869C(&self->data->field_24);
    func_08B45AB4(&self->data->field_28);
    func_08920A7C(self->data->field_1C, arg5);
    func_08B46F00(self);
}
