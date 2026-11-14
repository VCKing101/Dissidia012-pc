#include "menu/auto_region4.h"

void func_08B46B30(Net45BB8 *self)
{
    int value;
    NetData45BB8 *data;

    func_08B46AEC(self);
    value = func_089212E8(self->data->field_1C);
    self->data->field_16C = value;
    data = self->data;
    value = func_08B495D4(&data->field_24, data->field_16C, data->field_170);
    self->data->field_168 = value;
}
