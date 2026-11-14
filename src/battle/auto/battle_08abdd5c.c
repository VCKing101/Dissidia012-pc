#include "battle/auto_region4.h"

/* PartBDE18: field_48 of what func_08ABC300 finds for field_04 (kind 3),
 * or 0. */
int func_08ABDD5C(PartBDE18 *self)
{
    EntryBC300 *entry;

    entry = func_08ABC300(self->field_04, 3);
    if (entry == 0) {
        return 0;
    } else {
        return entry->field_48;
    }
}
