#include "main/auto_region2.h"

/* The short at (row, column) of the table D_089D7354, or -1 without one. */
int func_08887928(Cell887928 *self)
{
    int result;
    Row887928 *row;

    row = &D_089D7354[self->row];
    result = -1;
    if (row != 0) {
        result = row->values[self->column];
    }
    return result;
}
