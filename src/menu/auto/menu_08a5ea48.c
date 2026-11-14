#include "menu/auto_region2.h"

/* Panel5EED8: sets up its two cursors (index of four, then one of two). */
void func_08A5EA48(Panel5EED8 *self, int index)
{
    func_088D46E4(&self->field_F8, index, 0, 3, 0, -1);
    func_088D46E4(&self->field_124, 0, 0, 1, 0, -1);
    self->field_124.flags = self->field_124.flags | 1;
}
