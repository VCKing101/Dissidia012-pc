#include "menu/auto_region3.h"

/* Screen7EEDC: index of the entry with this id, or -1. */
int func_08A7F280(Screen7EEDC *self, int id)
{
    Work7EEDC *work;
    int i;

    work = self->work;
    i = 0;
    do {
        if (work->entries[i].id == id) {
            return i;
        }
        i = i + 1;
    } while (i < 0x28);
    return -1;
}
