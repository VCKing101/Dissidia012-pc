#include "menu/agent20_types.h"

/* Runs func_089F2110 on every kind-1 entry with flag 0x800. */
extern void func_089F2110(A20EntryOwner *self, A20Entry *entry);

void func_089F217C(A20EntryOwner *self)
{
    int i;
    for (i = 0; i < self->field_14C->field_2731; i++) {
        if (self->field_14C->field_29CC[i].field_00 == 1 && (self->field_14C->field_29CC[i].field_04 & 0x800) != 0) {
            func_089F2110(self, &self->field_14C->field_29CC[i]);
        }
    }
}
