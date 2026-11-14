#include "battle/auto_region5.h"

/* Is `id` one of 0x1C, 0x1D and 0x1E? */
unsigned char func_08B1CD40(void *self, int id)
{
    return (id == 0x1C || id == 0x1D) || id == 0x1E;
}
