#include "menu/auto_region2.h"

/* Panel61C40: keeps messages and sets itself up, state 1. */
void func_08A6108C(Panel61C40 *self, void *messages, unsigned char flag)
{
    self->messages = messages;
    func_08A610C8(self, flag);
    func_08A61240(self);
    self->state = 1;
}
