#include "main/auto_region2.h"

/* Whether flag 0x2000 of flags_384 is set. */
unsigned char func_088378E8(ActorView *self)
{
    return (self->flags_384 & 0x2000) != 0;
}
