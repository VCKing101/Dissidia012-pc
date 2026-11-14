/* Appends an entry to list `list`. */
#include "menu/agent24_types.h"

void func_08ACDE24(A24ListScreen *self, int list, short *id, short a, int b,
                   unsigned char flags, unsigned char mask)
{
    self->field_DB30->lists[list].entries[self->field_DB30->counts[list]].id = *id;
    self->field_DB30->lists[list].entries[self->field_DB30->counts[list]].field_2 = a;
    self->field_DB30->lists[list].entries[self->field_DB30->counts[list]].field_4 = b;
    self->field_DB30->lists[list].entries[self->field_DB30->counts[list]].flags = flags;
    self->field_DB30->lists[list].entries[self->field_DB30->counts[list]].mask = mask;
    self->field_DB30->counts[list]++;
}
