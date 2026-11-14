#include "battle/auto_region5.h"

/* Part437D4 constructor: field_00 is -1; builds the 0x20 entries. */
Part437D4 *func_08B437D4(Part437D4 *self)
{
    self->field_00 = -1;
    func_08993560(self->entries, 0x20, 0x14, func_08B436C0);
    return self;
}
