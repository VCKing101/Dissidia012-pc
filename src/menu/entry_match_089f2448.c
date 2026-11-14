#include "menu/agent20_types.h"

/* Flags (0x40000) every open kind-6 entry with the given group and id; returns 1 if any matched. */
int func_089F2448(A20EntryOwner *self, int group, short id)
{
    int i;
    int found = 0;
    for (i = 0; i < self->field_14C->field_2731; i++) {
        if (self->field_14C->field_29CC[i].field_00 == 6 && (self->field_14C->field_29CC[i].field_04 & 0x8000) == 0 &&
            self->field_14C->field_29CC[i].field_0F == group && self->field_14C->field_29CC[i].field_10 == id) {
            self->field_14C->field_29CC[i].field_04 |= 0x40000;
            found = 1;
        }
    }
    return found;
}
