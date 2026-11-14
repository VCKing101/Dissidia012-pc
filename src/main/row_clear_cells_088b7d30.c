/* Clears the twelve single cells of a grid row to 0xFF. */
#include "main/agent17_types.h"
void func_088B7D30(GridRow *row)
{
    int i;
    for (i = 0; i < 12; i++) {
        *((unsigned char *)row + i) = 0xFF;
    }
}
