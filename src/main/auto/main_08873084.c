#include "main/auto_region2.h"

/* Whether D_089D6008 has flag 0x2. */
unsigned char func_08873084(void)
{
    return (D_089D6008->flags & 2) != 0;
}
