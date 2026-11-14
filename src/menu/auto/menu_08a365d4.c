#include "menu/auto_region1.h"

/* Screen38EA8: lays out its seven rows (field_464, 0x18 bytes each).
 * The row address is written as work + offset + 0x464 (the offset of
 * field_464), as retail computes it: indexing field_464[i] adds in the other
 * order and swaps two saved registers. */
void func_08A365D4(Screen38EA8 *self)
{
    int i;
    int offset;

    i = 0;
    offset = 0;
    do {
        func_08A41454(D_08A457EC, (char *)self->work + offset + 0x464);
        i++;
        offset += sizeof(self->work->field_464[0]);
    } while (i < 7);
}
