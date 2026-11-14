/* Sets the current index on the board and on each of its grids. */
#include "main/agent17_types.h"

extern "C" {

void func_088B83F0(Board *board, int current)
{
    board->current = current;
    for (int i = 0; i < 5; i++) {
        board->grids[i].current = current;
    }
}

}
