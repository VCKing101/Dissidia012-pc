#include "menu/auto_region2.h"

/* Screen6D710: sets its task's callbacks, hands it its first 0x100 bytes and starts it. */
void func_08A6CF2C(Screen6D710 *self)
{
    MenuTask *task;

    task = &self->task;
    func_0886503C(task, func_08A6D284, func_08A6D2A4, 0);
    func_0886504C(task, self, 0x100);
    func_088650A4(task);
}
