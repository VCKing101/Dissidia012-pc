#include "main/auto_region2.h"

/* The node after prev (the first one when prev is 0). */
Node87A5E0 *func_0887A5E0(Obj87A4FC *self, Node87A5E0 *prev)
{
    return (prev == 0) ? self->first : prev->next;
}
