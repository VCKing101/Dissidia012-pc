#include "menu/auto_region2.h"

/* Panel69A78: opens with messages, fills its prompt. */
void func_08A69714(Panel69A78 *self, void *messages, int arg)
{
    self->messages = messages;
    self->field_34 = arg;
    func_08A698F4(self);
    func_08A69938(self);
    func_08A69A20(self);
    self->state = 0;
    self->field_2C = 1;
}
