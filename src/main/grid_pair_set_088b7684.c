/* Sets both bytes of grid pair i. */
#include "main/agent17_types.h"

void func_088B7684(Grid *grid, int i, int first, int second)
{
    grid->pairs[i].first = first;
    grid->pairs[i].second = second;
}
