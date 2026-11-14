/* Sets a triple of one grid row. */
#include "main/agent17_types.h"

void func_088B7DA4(GridRow *row, int triple, int index, int value);

void func_088B7618(Grid *grid, int row, int triple, int index, int value)
{
    func_088B7DA4(&grid->rows[row], triple, index, value);
}
