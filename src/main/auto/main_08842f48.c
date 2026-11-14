#include "main/auto_region2.h"

/* Whether value is at most percent + 20 percent of total. */
unsigned char func_08842F48(int value, int total, int percent)
{
    return (value * 100) / total <= percent + 0x14;
}
