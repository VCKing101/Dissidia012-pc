#include "battle/agent14_types.h"

extern "C" {

extern void func_08A86A20(LinkSlot *slot);
extern void func_08A86D58(LinkSet *self);

void func_08A86D94(LinkSet *self)
{
    for (int i = 0; i < 3; i++) {
        func_08A86A20(&self->slots[i]);
    }
    func_08A86D58(self);
}

}
