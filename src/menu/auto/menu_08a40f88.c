#include "menu/auto_region2.h"

/* MenuRoot2: sets its task's callbacks and hands it its first 0x100 bytes. */
void func_08A40F88(MenuRoot2 *self)
{
    func_0886503C(&self->task, func_08A42BBC, func_08A42BDC, 0);
    func_0886504C(&self->task, self, 0x100);
}
