/* Stores `value` in the cell at position `pos` of the first list and marks
   the lists dirty. */
#include "menu/agent24_types.h"

void func_08B0F370(A24CellOwner *self, int pos, short value)
{
    self->field_138->field_3264[0].cells[self->field_138->field_1A964[0][pos]].field_4 = value;
    self->field_138->field_24820 = 1;
}
