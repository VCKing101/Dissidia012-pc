#include "battle/auto_region5.h"

/* Is `id` outside 4..7? */
unsigned char func_08B2A4F8(Part2BAF0 *self, int id)
{
    return ((id != 4 && id != 5) && id != 6) && id != 7;
}
