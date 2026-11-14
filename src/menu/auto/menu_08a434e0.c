#include "menu/auto_region2.h"

/* Screen457F0: sets its task's callbacks and hands it its first 0x100 bytes. */
void func_08A434E0(Screen457F0 *self)
{
    func_0886503C(&self->task, func_08A43660, func_08A43680, 0);
    func_0886504C(&self->task, self, 0x100);
}
