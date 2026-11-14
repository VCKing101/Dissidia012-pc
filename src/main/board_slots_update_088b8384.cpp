/* Updates every used slot of the board. */
#include "main/agent17_types.h"

extern "C" {

void func_088B830C(Board *board, int id);

void func_088B8384(Board *board)
{
    for (int i = 0; i < 150; i++) {
        BoardSlot *slot = &board->slots[i];
        if (slot->id != -1) {
            func_088B830C(board, slot->id);
        }
    }
}

}
