/* Counts the entries of list `list` (in display order) usable in the current
   mode: mask bit (1 << mode) << 4 set and flag bit 3 clear. */
#include "menu/agent24_types.h"

int func_08ACE8D8(A24ListScreen *self, int list)
{
    int usable = 0;
    unsigned char bit = (1 << self->field_DB30->field_1214) << 4;
    int i;
    for (i = 0; i < self->field_DB30->counts[list]; i++) {
        if ((self->field_DB30->lists[list].entries[self->field_DB30->order[list][i]].mask & bit) != 0
            && (self->field_DB30->lists[list].entries[self->field_DB30->order[list][i]].flags & 8) == 0) {
            usable++;
        }
    }
    return usable;
}
