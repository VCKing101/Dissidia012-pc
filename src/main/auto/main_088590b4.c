#include "main/auto_region2.h"

/* Releases the five handles and clears the two counters. */
void func_088590B4(Part859060 *self)
{
    int i;

    i = 0;
    do {
        if (self->handles[i] != 0) {
            func_0899BB1C(self->handles[i]);
        }
        self->handles[i] = 0;
        i = i + 1;
    } while (i < 5);
    self->field_2A4 = 0;
    self->field_2A0 = 0;
}
