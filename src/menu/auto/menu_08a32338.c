#include "menu/auto_region1.h"

/* Screen35A00: func_08A31688, then stops its task. */
void func_08A32338(Screen35A00 *self)
{
    func_08A31688(self);
    func_08865158(&self->task);
}
