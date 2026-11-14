/* Sets (or, with `clear`, clears) flag 0x400 on every cell of the current
   list and marks it dirty. */
#include "menu/agent24_types.h"

void func_08B0F1D4(A24CellOwner *self, unsigned char clear)
{
    int list = self->field_138->field_1F8EC;
    int i;
    for (i = 0; i < self->field_138->field_31BC[list]; i++) {
        if (clear) {
            self->field_138->field_3264[list].cells[i].field_2 &= ~0x400;
        } else {
            self->field_138->field_3264[list].cells[i].field_2 |= 0x400;
        }
    }
    self->field_138->field_24820 = 1;
}
