/* True when rows 36..38 of the current grid page hold at least 64 used
   cells (first byte not 0xFF). */
#include "menu/agent24_types.h"

unsigned char func_08AD0068(A24GridScreen *self)
{
    A24GridState *state = self->field_DB30;
    unsigned char full = 0;
    int used = 0;
    int row;
    int col;
    for (row = 0x24; row < 39; row++) {
        for (col = 0; col < 64; col++) {
            if (state->pages[state->field_1210].cells[row][col].a != 0xFF) {
                used++;
            }
        }
    }
    if (used >= 0x40) {
        full = 1;
    }
    return full;
}
