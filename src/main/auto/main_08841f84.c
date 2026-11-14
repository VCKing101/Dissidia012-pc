#include "main/auto_region2.h"

/* The object on the world's +0x14 list after prev (the first one when prev is 0). */
Fighter14 *func_08841F84(Fighter14 *prev)
{
    return (prev == 0) ? D_089D4958->first_14 : prev->next;
}
