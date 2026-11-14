#include "menu/auto_region1.h"

/* Screen0B6A0: clears flags 0xF80E of entry and sets its kind. */
void func_089F5BFC(Screen0B6A0 *self, Entry84 *entry, unsigned char kind)
{
    entry->flags = entry->flags & 0xFFFF07F1;
    entry->kind = kind;
}
