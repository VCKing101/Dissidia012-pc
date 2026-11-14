/* Copies the current grid page into the work grid at 0xAB44. */
#include "menu/agent24_types.h"

void func_08ACEE3C(A24GridScreen *self)
{
    int row;
    int col;
    for (row = 0; row < 39; row++) {
        for (col = 0; col < 64; col++) {
            self->field_DB30->field_AB44.cells[row][col] = self->field_DB30->pages[self->field_DB30->field_1210].cells[row][col];
        }
    }
}
