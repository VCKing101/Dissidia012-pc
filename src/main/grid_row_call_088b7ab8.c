/* Runs a row operation with the grid's current index. */
#include "main/agent17_types.h"

void func_088B7DF0(GridRow *row, int current, int a, int b);

void func_088B7AB8(Grid *grid, int row, int a, int b)
{
    func_088B7DF0(&grid->rows[row], grid->current, a, b);
}
