#include "main/auto_region2.h"

/* Writes the "ARC" 1 header: points at itself, no entries. */
void func_0885CAFC(Arc85CAFC *self)
{
    self->magic[0] = 'A';
    self->magic[1] = 'R';
    self->magic[2] = 'C';
    self->magic[3] = 1;
    self->field_08 = self;
    self->field_04 = 0;
}
