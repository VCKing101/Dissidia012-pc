/* Clears the 64 extra cells to 0xFF. */
#include "main/agent17_types.h"
void func_088B758C(Grid *grid)
{
    int i;
    for (i = 0; i < 64; i++) {
        grid->extra[i] = 0xFF;
    }
}
