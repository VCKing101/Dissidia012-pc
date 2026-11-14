/* Toggles flag 0x400 of the cell at position `pos` and marks the list dirty. */
#include "menu/agent24_types.h"

void func_08B0F144(A24CellOwner *self, int pos)
{
    unsigned short index = self->field_138->field_1A964[self->field_138->field_1F8EC][pos];
    self->field_138->field_3264[self->field_138->field_1F8EC].cells[index].field_2 ^= 0x400;
    self->field_138->field_24820 = 1;
}
