#include "menu/auto_region4.h"

/* ScreenB54C38: takes its arguments and starts its task. */
void func_08B54184(ScreenB54C38 *self, unsigned int value, unsigned char arg1, unsigned char arg2, unsigned char arg3)
{
    MenuTask *task;

    self->field_148 = value;
    self->field_14C = arg1;
    self->field_14D = arg2;
    self->field_14E = arg3;
    task = &self->task;
    func_0886503C(task, func_08B542E8, func_08B54308, 0);
    func_0886504C(task, self->block, 0x100);
    func_088650A4(task);
}
