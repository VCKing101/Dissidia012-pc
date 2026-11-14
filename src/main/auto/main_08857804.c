#include "main/auto_region2.h"

/* Marks the forty entries inactive. */
void func_08857804(Part8577D8 *self)
{
    int i;

    i = 0;
    do {
        self->entries[i].active = 0;
        i = i + 1;
    } while (i < 0x28);
}
