#include "main/auto_region2.h"

/* Drops a reference to the entry for id; the last one destroys it and frees it to the table's heap. */
void func_08844520(Table9D4960 *self, int id)
{
    Entry9D4960 *entry;
    int last;

    entry = func_088444D4(self, id);
    last = func_0884405C(entry);
    if (last == 0) {
        func_0884404C(entry);
    } else {
        func_08846628(entry);
        func_0885C538(self->heap, entry, 0);
    }
}
