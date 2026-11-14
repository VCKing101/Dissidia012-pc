#include "menu/auto_region3.h"

/* Obj74588 constructor: builds field_34 and clears the first 0x34 bytes. */
Obj74588 *func_08A74588(Obj74588 *self)
{
    func_0887B1E4(self->field_34);
    func_08935828(self, 0, 0x34);
    return self;
}
