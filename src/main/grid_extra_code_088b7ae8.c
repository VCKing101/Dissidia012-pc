/* Code of extra cell i: -1 when empty (0xFF), otherwise the cell value with bit 14 set. */
#include "main/agent17_types.h"
int func_088B7AE8(Grid *grid, int i)
{
    unsigned char *extra = grid->extra;
    int v = extra[i];
    if (v == 0xFF) {
        return -1;
    }
    return (unsigned char)v | 0x4000;
}
