#include "menu/auto_region3.h"

/* Screen87EAC: starts its task and keeps the value in D_09B6CD88. */
void func_08A86A2C(Screen87EAC *self, int value)
{
    MenuTask *task;

    task = &self->task;
    func_0886503C(task, func_08A86B30, func_08A86B50, 0);
    func_088650A4(task);
    func_0886504C(task, self, 0x100);
    D_09B6CD88 = value;
}
