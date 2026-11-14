#include "menu/auto_region3.h"

/* ScreenB01CF8: sets its task's callbacks and block. */
void func_08AF2CB0(ScreenB01CF8 *self)
{
    func_0886503C(&self->task, func_08AFA790, func_08AFA7B0, 0);
    func_0886504C(&self->task, self, 0x100);
}
