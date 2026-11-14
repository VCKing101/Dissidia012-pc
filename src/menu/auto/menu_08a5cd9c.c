#include "menu/auto_region2.h"

/* Panel5DB80: opens on entry and lays itself out, state 1. */
void func_08A5CD9C(Panel5DB80 *self, Entry10 *entry, void *messages)
{
    self->field_644 = entry;
    self->messages = messages;
    self->field_640 = func_088729A0(0);
    func_08A5D580(self);
    func_08A5CE80(self, self->messages);
    func_08A5D678(self);
    func_08A5D714(self);
    func_08A5DACC(self, self->field_644);
    self->state = 1;
}
