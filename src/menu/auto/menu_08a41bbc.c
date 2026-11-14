#include "menu/auto_region2.h"

/* MenuRoot2: sets bit number bit in its bit set. */
void func_08A41BBC(MenuRoot2 *self, unsigned int bit)
{
    unsigned int *bits;
    unsigned int *word;

    bits = self->work->bits;
    word = &bits[bit >> 5];
    *word = *word | 1 << (bit & 0x1F);
}
