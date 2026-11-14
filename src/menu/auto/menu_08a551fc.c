#include "menu/auto_region2.h"

/* Item: sets its cursor on index (over count entries) and flags it. */
void func_08A551FC(Item *self, int index)
{
    func_088D46E4(&self->cursor, index, 0, self->count - 1, 0, -1);
    self->cursor.flags = self->cursor.flags | 1;
}
