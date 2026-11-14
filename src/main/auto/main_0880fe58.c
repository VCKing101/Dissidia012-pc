#include "main/auto_region1.h"

/* Copies a matrix into self and sets flag bit 0. */
void func_0880FE58(Obj80FB6C *self, const float *matrix)
{
    func_089A3868(self->matrix, matrix);
    self->flags = self->flags | 1;
}
