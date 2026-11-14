#include "menu/auto_region4.h"

void func_08B3A1CC(ScreenB41CC0 *self, int arg)
{
    int value;
    void *object;

    value = func_08B45200(arg);
    self->flags = self->flags & 0xFFFFFFBF;
    object = self->field_C4->field_10;
    func_08B46584(object, value);
    func_08B46840(object);
    func_08B46698(object, 1);
    func_0892D380(self->field_C8->field_1C, self->field_C8->messages, 0xB, 1);
    func_08B342D4(self->field_C4->field_0C, 1);
}
