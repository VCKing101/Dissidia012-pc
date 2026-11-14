#include "main/auto_region2.h"

/* Releases and clears the five ids (func_089C5810(3) before the last). */
void func_08863E40(Obj9C8FE0 *self)
{
    func_089C5F90(self->field_00);
    func_089C5F90(self->field_04);
    func_089C5F90(self->field_08);
    func_089C5F90(self->field_0C);
    self->field_00 = 0;
    self->field_04 = 0;
    self->field_08 = 0;
    self->field_0C = 0;
    func_089C5810(3);
    func_089C5F90(self->field_10);
    self->field_10 = 0;
}
