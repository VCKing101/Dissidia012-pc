#include "menu/auto_region4.h"

/* ScreenB29CF8: hands the task its callbacks, starts it and hands it its block. */
void func_08B28BB4(ScreenB29CF8 *self)
{
    MenuTask *task;

    task = &self->task;
    func_0886503C(task, func_08B28CA4, func_08B28CC4, 0);
    func_088650A4(task);
    func_0886504C(task, self->block, 0x100);
}
