#include "main/auto_region2.h"

/* The actor after prev (the first one when prev is 0). */
Actor *func_08841F64(Actor *prev)
{
    return (prev == 0) ? D_089D4958->actors : prev->next;
}
