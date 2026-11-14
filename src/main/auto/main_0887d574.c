#include "main/auto_region2.h"

/* Constructor of Obj87D574: builds the entries, clears head and table, field_310 and the entries. */
Obj87D574 *func_0887D574(Obj87D574 *self)
{
    func_08993560(self->entries, 0x1c, 500, &D_0887D534);
    func_0887D60C(self);
    func_0887D674(self);
    func_0887D648(self);
    return self;
}
