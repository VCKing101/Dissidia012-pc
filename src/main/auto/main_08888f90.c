#include "main/auto_region2.h"

/* Frees the block to D_089D69A0's second heap and clears it. */
void func_08888F90(Holder *self)
{
    func_08882158(self->value);
    self->value = 0;
}
