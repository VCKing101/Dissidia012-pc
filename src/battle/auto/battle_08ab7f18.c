#include "battle/auto_region4.h"

/* PartB7F18 constructor (no vtable): clears field_00 and the eight words
 * at field_24. */
PartB7F18 *func_08AB7F18(PartB7F18 *self)
{
    int i;

    self->field_00 = 0;
    for (i = 0; i < 8; i++) {
        self->field_24[i] = 0;
    }
    return self;
}
