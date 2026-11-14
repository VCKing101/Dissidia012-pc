#include "main/auto_region2.h"

/* The object on the world's +0x24 list after prev (the first one when prev is 0). */
Obj24 *func_08842068(Obj24 *prev)
{
    return (prev == 0) ? D_089D4958->first_24 : prev->next;
}
