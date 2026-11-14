/* Code of triple t, cell i of a row: -1 when empty (0xFF), otherwise current << 8 | value. */
#include "main/agent17_types.h"
int func_088B7DF0(GridRow *row, int current, int t, int i)
{
    unsigned char (*triples)[3] = row->triples;
    int v = triples[t][i];
    if (v == 0xFF) {
        return -1;
    }
    return (current << 8) | (unsigned char)v;
}
