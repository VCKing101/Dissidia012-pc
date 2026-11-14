#include "main/auto_region1.h"

/* Reset of Obj9C6D60: clears the flag, field_04 and the 0x1C bytes from field_08. */
void func_088105B4(Obj9C6D60 *self)
{
    self->active = 0;
    self->field_04 = 0;
    func_08935828(&self->field_08, 0, 0x1c);
}
