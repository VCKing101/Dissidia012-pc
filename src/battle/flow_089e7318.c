#include "battle/runtime_helpers.h"

/* Connected slot/binding/update flow; full class ownership remains unknown. */
extern int func_089E752C(void *);

int func_089E7318(unsigned char *object)
{
    if (!object[0]) return -1;
    {
        int i = 0;
        unsigned char *entry = object + 4;
        do {
            if (func_089E752C(entry)) return -2;
            ++i;
            entry += 0x20;
        } while (i < 2);
    }
    return 0;
}
