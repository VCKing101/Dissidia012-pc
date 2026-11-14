#include "menu/auto_region2.h"

/* Screen51CBC: sets its task's callbacks and starts it. */
void func_08A50270(Screen51CBC *self)
{
    func_0886503C(&self->task, func_08A5018C, func_08A5021C, func_08A5023C);
    func_088650A4(&self->task);
}
