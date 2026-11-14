#include "main/auto_region2.h"

/* Counts a use of every entry. */
void func_08844A60(Table9D4960 *self)
{
    Entry9D4960 *entry;

    entry = 0;
    while (1) {
        entry = (entry == 0) ? self->first : entry->next;
        if (entry == 0) break;
        func_08843F4C(entry);
    }
}
