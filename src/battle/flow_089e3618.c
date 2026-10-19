#include "battle/runtime_helpers.h"

/* Connected slot/binding/update flow; full class ownership remains unknown. */


int func_089E3618(unsigned char *object)
{
    if (!object[0]) return -1;
    {
        int i = 0;
        unsigned char *entry = object + 8;
        do {
            func_089E3BF0(entry);
            ++i;
            entry += 0xD8;
        } while (i < 2);
    }
    return 0;
}
