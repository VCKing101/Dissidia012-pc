#include "menu/auto_region3.h"

/* ObjB41E0: sets or clears flag 0x4000000. */
void func_08AB6D20(ObjB41E0 *self, unsigned char on)
{
    unsigned int flags;

    if (on == 1) {
        flags = self->flags | 0x4000000;
    }
    else {
        flags = self->flags & 0xFBFFFFFF;
    }
    self->flags = flags;
}
