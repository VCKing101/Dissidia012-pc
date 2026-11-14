#include "main/auto_region1.h"

/* Sets (on == 1) or clears the given flag bits. */
void func_0880FEA0(Obj80FB6C *self, unsigned int bits, unsigned char on)
{
    if (on == 1) {
        bits = self->flags | bits;
    } else {
        bits = self->flags & ~bits;
    }
    self->flags = bits;
}
