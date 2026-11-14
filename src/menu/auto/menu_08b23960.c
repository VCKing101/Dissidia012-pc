#include "menu/auto_region4.h"

/* ScreenB243C0: hands the task its callbacks and block. */
void func_08B23960(ScreenB243C0 *self)
{
    func_0886503C(&self->task, func_08B23CB4, func_08B23CD4, 0);
    func_0886504C(&self->task, self->block, 0x100);
}
