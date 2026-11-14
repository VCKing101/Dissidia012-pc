#include "main/auto_region2.h"

/* The object on the world's +0x34 list after prev (the first one when prev is 0). */
Obj34 *func_088420A8(Obj34 *prev)
{
    return (prev == 0) ? D_089D4958->first_34 : prev->next;
}
