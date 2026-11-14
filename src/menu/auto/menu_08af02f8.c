#include "menu/auto_region3.h"

/* Position of a cell: x = column * 13, y = (row % 3) * -16. */
void func_08AF02F8(Point8 *out, int column, int row)
{
    out->x = column * 0xD;
    out->y = (row % 3) * -0x10;
}
