#include "battle/agent14_types.h"

extern "C" {

extern void func_08A86D94(LinkSet *self);
extern int func_08A86A3C(LinkSlot *slot, void *owner);

int func_08A86DEC(LinkSet *self, void *owner, void *data)
{
    func_08A86D94(self);
    if (owner == 0 || data == 0) {
        return -1;
    }
    for (int i = 0; i < 3; i++) {
        if (func_08A86A3C(&self->slots[i], owner) != 0) {
            return -2;
        }
    }
    self->field_B0 = 0;
    self->owner = owner;
    self->data = data;
    self->active = 1;
    return 0;
}

}
