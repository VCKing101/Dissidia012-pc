/* Runs a row operation with the grid's current index. */
#include "main/agent17_types.h"

void func_088B7DC0(GridRow *row, int current, int arg);

void func_088B7A88(Grid *grid, int row, int arg)
{
    func_088B7DC0(&grid->rows[row], grid->current, arg);
}
