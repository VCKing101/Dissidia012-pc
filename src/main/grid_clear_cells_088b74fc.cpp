/* Clears the twelve single cells of every grid row to 0xFF. */
#include "main/agent17_types.h"
extern "C" {
void func_088B74FC(Grid *grid)
{
    for (int r = 0; r < 3; r++) {
        for (int i = 0; i < 12; i++) {
            grid->rows[r].cells[i] = 0xFF;
        }
    }
}
}
