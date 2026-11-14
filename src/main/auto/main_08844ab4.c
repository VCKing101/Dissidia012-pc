#include "main/auto_region2.h"

/* Calls func_08843F88 on every entry. */
void func_08844AB4(Table9D4960 *self)
{
    Entry9D4960 *entry;

    entry = 0;
    while (1) {
        entry = (entry == 0) ? self->first : entry->next;
        if (entry == 0) break;
        func_08843F88(entry);
    }
}
