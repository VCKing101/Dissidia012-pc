#include "menu/auto_region1.h"

/* Screen0B6A0: swaps flag 0x1000 for 0x2000 and clears field_0C/0E of entry. */
void func_089F1A08(Screen0B6A0 *self, Entry84 *entry)
{
    entry->flags = entry->flags & ~0x1000 | 0x2000;
    entry->field_0C = 0;
    entry->field_0E = 0;
}
