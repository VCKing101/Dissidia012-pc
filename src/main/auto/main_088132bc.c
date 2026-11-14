#include "main/auto_region1.h"

/* Whether bit 1 of the actor's flags_08 is set. */
unsigned char func_088132BC(ActorPart *self)
{
    return (self->actor->flags_08 & 2) != 0;
}
