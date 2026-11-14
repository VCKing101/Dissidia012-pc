#include "menu/auto_region2.h"

/* Panel5EED8: lays itself out with messages and sets up its Panel60670. */
void func_08A5E7EC(Panel5EED8 *self, void *messages)
{
    self->messages = messages;
    func_08A5E848(self);
    func_08A5E94C(self, messages);
    func_08A602A4(&self->field_A0, self->handles[0], 1, self->messages, 8, -1);
}
