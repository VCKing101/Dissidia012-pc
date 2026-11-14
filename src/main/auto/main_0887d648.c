#include "main/auto_region2.h"

/* Marks the twenty-eight entries inactive with a zero count. */
void func_0887D648(Obj87D574 *self)
{
    int i;

    i = 0;
    do {
        self->entries[i].active = 0;
        self->entries[i].count = 0;
        i = i + 1;
    } while (i < 0x1c);
}
