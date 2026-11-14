/* Stores the picked slot and refreshes. */
#include "menu/agent24_types.h"

extern void func_08B2DE8C(A24SaveOwner *self);

void func_08B2DE6C(A24SaveOwner *self, int slot)
{
    self->field_844->field_6A0 = slot;
    func_08B2DE8C(self);
}
