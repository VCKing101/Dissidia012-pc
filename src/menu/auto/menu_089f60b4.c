#include "menu/auto_region1.h"

/* Screen0B6A0: 1 when entry's kind is not 4..7 and none of flags 0x80C is
 * set. */
unsigned int func_089F60B4(Screen0B6A0 *self, Entry84 *entry)
{
    unsigned char kind;
    unsigned int result;

    kind = entry->kind;
    result = 0;
    if ((((kind != 4) && (kind != 5)) && (kind != 6)) &&
        ((kind != 7) && ((entry->flags & 0x80C) == 0))) {
        result = 1;
    }
    return result;
}
