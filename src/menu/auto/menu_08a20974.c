#include "menu/auto_region1.h"

/* Menu root: address of row index of field_1E48. */
Row154 *func_08A20974(MenuRoot *self, int index)
{
    return &self->work->field_1E48[index];
}
