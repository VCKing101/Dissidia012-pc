#include "main/auto_region2.h"

/* Whether actor flag 0x8 is set. */
unsigned char func_088377C0(ActorView *self)
{
    return (self->flags_380 & 8) != 0;
}
