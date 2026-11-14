#include "main/auto_region2.h"

/* Clears field_10 and the two counters. */
void func_08851C28(Part851C28 *self)
{
    func_08935828(self->field_10, 0, 0x3c0);
    self->field_3D4 = 0;
    self->field_3D0 = 0;
}
