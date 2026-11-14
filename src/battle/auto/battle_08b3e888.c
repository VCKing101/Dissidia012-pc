#include "battle/auto_region5.h"

/* Is flag 0x100 set on what func_08B3E8B0 returns? */
unsigned char func_08B3E888(void)
{
    Node3E888 *node;

    node = func_08B3E8B0();
    return (node->flags & 0x100) != 0;
}
