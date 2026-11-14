#include "main/auto_region2.h"

/* Keeps a copy of record and sets field_60F. */
void func_08850FF4(Obj850FF4 *self, const Record9D4968 *record)
{
    func_08935524(&self->record, record, 0x44);
    self->field_60F = 1;
}
