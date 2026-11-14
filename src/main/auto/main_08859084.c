#include "main/auto_region2.h"

/* Sets field_2A8, marks the ten entries inactive and clears the two counters. */
void func_08859084(Part859060 *self)
{
    int i;

    self->field_2A8 = 1;
    i = 0;
    do {
        self->entries[i].active = 0;
        i = i + 1;
    } while (i < 10);
    self->field_2A4 = 0;
    self->field_2A0 = 0;
}
