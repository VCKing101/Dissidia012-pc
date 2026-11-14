#include "menu/auto_region4.h"

/* ScreenB1CB00: hands the task its callbacks and block. */
void func_08B0C074(ScreenB1CB00 *self)
{
    func_0886503C(&self->task, func_08B0C340, func_08B0C360, 0);
    func_0886504C(&self->task, self->block, 0x100);
}
