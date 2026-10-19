#include "battle/runtime_helpers.h"

/* Connected slot/binding/update flow; full class ownership remains unknown. */

int func_089E1FD0(unsigned char *object)
{
    if (!object[0]) return -1;
    {
        int i = 0;
        unsigned char *entry_a = object + 0xC;
        unsigned char *entry_b = object + 0x44;
        do {
            func_089E2BB4(entry_a);
            func_089E2418(entry_b);
            ++i;
            entry_a += 0x1C;
            entry_b += 0x68;
        } while (i < 2);
    }
    return 0;
}
