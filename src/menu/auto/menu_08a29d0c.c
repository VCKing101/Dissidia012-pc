#include "menu/auto_region1.h"

/* ScreenE75C8: sets up its task (callbacks func_08A2BC64 / func_08A2BC84). */
void func_08A29D0C(ScreenE75C8 *self)
{
    func_0886503C(&self->task, func_08A2BC64, func_08A2BC84, 0);
    func_0886504C(&self->task, self, 0x100);
}
