#include "main/auto_region2.h"

/* Clears slot index of the four per-slot arrays (-1 in the last two). */
void func_08889904(Flow9D6A10 *self, int index)
{
    self->field_38[index] = 0;
    self->field_40[index] = 0;
    self->field_7D8[index] = 0xffffffff;
    self->field_7F0[index] = 0xffffffff;
}
