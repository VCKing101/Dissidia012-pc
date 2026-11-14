#include "menu/auto_region3.h"

extern const IdEntry8 D_08AC42B6[];

/* Index of id in the seven-entry table D_08AC42B6, or 0. */
int func_08ABC934(ScreenC4560 *self, unsigned int id)
{
    int i;
    const IdEntry8 *entry;

    i = 0;
    entry = D_08AC42B6;
    do {
        if (entry->id == id) {
            return i;
        }
        i = i + 1;
        entry = entry + 1;
    } while (i < 7);
    return 0;
}
