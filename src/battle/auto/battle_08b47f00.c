#include "battle/auto_region5.h"

/* Part4785C: returns field_04 of the entry field_4F0 picks. */
int func_08B47F00(Part4785C *self)
{
    return func_08B475DC(&self->entries[self->field_4F0]);
}
