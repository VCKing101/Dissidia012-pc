#include "main/auto_region2.h"

/* Constructor of Part85697C: clears the fourteen pairs. */
Part85697C *func_0885697C(Part85697C *self)
{
    Words2 *pair;
    int i;

    i = 0;
    pair = self->pairs;
    do {
        pair->field_00 = 0;
        pair->field_04 = 0;
        i = i + 1;
        pair = pair + 1;
    } while (i < 0xe);
    return self;
}
