#include "menu/auto_region3.h"

/* Screen8B7A8: starts its task, runs func_08A8AF60 and keeps two values. */
void func_08A88EDC(Screen8B7A8 *self, int arg1, int arg2)
{
    MenuTask *task;

    task = &self->task;
    func_0886503C(task, func_08A8908C, func_08A890AC, 0);
    func_088650A4(task);
    func_0886504C(task, self, 0x100);
    func_08A8AF60(self);
    self->field_304 = arg1;
    self->field_308 = arg2;
}
