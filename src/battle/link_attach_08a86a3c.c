#include "battle/agent14_types.h"

extern void func_08A86A20(LinkSlot *self);

int func_08A86A3C(LinkSlot *self, void *owner)
{
    func_08A86A20(self);
    if (owner == 0) {
        return -1;
    }
    self->owner = owner;
    self->active = 1;
    return 0;
}
