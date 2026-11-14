/* Stores the value in the first free (-1) of the first ten grid values; 1 when stored. */
#include "main/agent17_types.h"
extern "C" {
int func_088B78BC(Grid *grid, int value)
{
    for (int i = 0; i < 10; i++) {
        if (grid->values[i] != -1) {
            continue;
        }
        grid->values[i] = value;
        return 1;
    }
    return 0;
}
}
