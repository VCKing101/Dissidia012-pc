/* Code of single cell i of a row: -1 when empty (0xFF), otherwise current << 8 | value. */
#include "main/agent17_types.h"
int func_088B7DC0(GridRow *row, int current, int i)
{
    int v = *((unsigned char *)row + i);
    if (v == 0xFF) {
        return -1;
    }
    return (current << 8) | (unsigned char)v;
}
