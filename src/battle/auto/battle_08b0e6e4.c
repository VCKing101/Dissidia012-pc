#include "battle/auto_region5.h"

/* Part0E63C: takes entry `index` of `table` and starts on it. */
void func_08B0E6E4(Part0E63C *self, void *table, int index)
{
    self->table = table;
    self->field_1C = 0;
    self->field_20 = func_08B0D0F4(table, index);
    func_08B0E804(self);
}
