#include "menu/auto_region2.h"

/* Screen4FFF4: sets its task's callbacks and starts it. */
void func_08A4C304(Screen4FFF4 *self)
{
    func_0886503C(&self->task, func_08A4C128, func_08A4C180, func_08A4C1A0);
    func_088650A4(&self->task);
}
