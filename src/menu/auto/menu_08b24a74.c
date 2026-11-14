#include "menu/auto_region4.h"

/* ScreenB25044: hands the task its callbacks and block. */
void func_08B24A74(ScreenB25044 *self)
{
    func_0886503C(&self->task, func_08B24E38, func_08B24E58, 0);
    func_0886504C(&self->task, self->block, 0x100);
}
