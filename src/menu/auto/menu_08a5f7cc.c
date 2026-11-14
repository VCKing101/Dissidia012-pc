#include "menu/auto_region2.h"

/* Panel5FA00: sets up its two cursors (7 and 13 entries). */
void func_08A5F7CC(Panel5FA00 *self)
{
    func_088D46E4(&self->field_324, 0, 0, 6, 0, -1);
    func_088D46E4(&self->field_350, 0, 0, 0xC, 0, -1);
    self->field_350.flags = self->field_350.flags | 1;
}
