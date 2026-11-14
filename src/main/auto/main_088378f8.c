#include "main/auto_region2.h"

/* Whether flag 0x4000 of flags_384 is set. */
unsigned char func_088378F8(ActorView *self)
{
    return (self->flags_384 & 0x4000) != 0;
}
