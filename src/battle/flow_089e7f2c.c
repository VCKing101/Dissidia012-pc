#include "battle/runtime_helpers.h"

/* Connected slot/binding/update flow; full class ownership remains unknown. */


int func_089E7F2C(unsigned char *object)
{
    if (!object[0]) return -1;
    {
        int i = 0;
        unsigned char *flags = object + 0x644;
        unsigned char *entry = object + 0x650;
        do {
            if (!flags[i]) func_089E5660(entry);
            ++i;
            entry += 0xDC;
        } while (i < 2);
    }
    return 0;
}
