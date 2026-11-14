#include "main/auto_region2.h"

/* Clears the five words. */
Obj8577B0 *func_088577B0(Obj8577B0 *self)
{
    int *word;
    int i;

    i = 0;
    word = self->field_00;
    do {
        *word = 0;
        i = i + 1;
        word = word + 1;
    } while (i < 4);
    self->field_10 = 0;
    return self;
}
