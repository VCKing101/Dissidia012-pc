#include "main/auto_region2.h"

/* Deleting destructor of the actor link. */
void func_0883E0AC(ActorLinkView *self, unsigned int flags)
{
    if (self != 0) {
        func_0883E1F0(self);
        self->field_00 = 0;
        func_0880ABF4(&self->bits, 2);
        if ((flags & 1) != 0) {
            func_08992F04(self);
        }
    }
}
