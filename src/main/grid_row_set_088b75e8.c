/* Sets a cell of one grid row. */
#include "main/agent17_types.h"

void func_088B7D94(GridRow *row, int cell, int value);

void func_088B75E8(Grid *grid, int row, int cell, int value)
{
    func_088B7D94(&grid->rows[row], cell, value);
}
