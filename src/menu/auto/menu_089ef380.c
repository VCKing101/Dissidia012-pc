#include "menu/auto_region1.h"

/* Index of the entry of D_08A0B1A8 (0x25 entries) whose field_02 is id;
 * 0 when there is none. */
int func_089EF380(Screen0B6A0 *self, short id)
{
    int found;
    IdPair *entry;
    unsigned int i;

    found = -1;
    i = 0;
    entry = D_08A0B1A8;
    do {
        if ((unsigned int)entry->field_02 == (int)id) {
            found = (short)i;
            break;
        }
        i = i + 1;
        entry = entry + 1;
    } while (i < 0x25);
    if (found == -1) {
        found = 0;
    }
    return found;
}
