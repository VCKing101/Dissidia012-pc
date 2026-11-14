/* Finds the entry `id` in list `list`, clears its bit 0 and sets bit 2. */
#include "menu/agent24_types.h"

void func_08ACDC98(A24ListScreen *self, int list, short id)
{
    int i;
    for (i = 0; i < self->field_DB30->counts[list]; i++) {
        if (self->field_DB30->lists[list].entries[i].id == id) {
            self->field_DB30->lists[list].entries[i].flags &= ~1;
            self->field_DB30->lists[list].entries[i].flags |= 4;
            break;
        }
    }
}
