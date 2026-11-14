#include "menu/auto_region1.h"

/* Screen0B6A0: clears flag 0x10000 and field_0C/0E of entry. */
void func_089F19E8(Screen0B6A0 *self, Entry84 *entry)
{
    entry->flags = entry->flags & ~0x10000;
    entry->field_0C = 0;
    entry->field_0E = 0;
}
