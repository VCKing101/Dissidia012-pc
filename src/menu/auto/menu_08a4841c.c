#include "menu/auto_region2.h"

/* IdTable: frees its rows. */
void func_08A4841C(IdTable *self)
{
    func_0885C538(self->heap, self->rows, 0);
    self->rows = 0;
}
