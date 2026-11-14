#include "menu/auto_region4.h"

/* ScreenB22E98: hands the task its callbacks and block. */
void func_08B1F124(ScreenB22E98 *self)
{
    func_0886503C(&self->task, func_08B1F534, func_08B1F554, 0);
    func_0886504C(&self->task, self->block, 0x100);
}
