#include "battle/runtime_helpers.h"

/* Connected slot/binding/update flow; full class ownership remains unknown. */

int func_089E0904(unsigned char *object)
{
    if (!func_089E0B9C(object)) return -1;
    {
        int i = 0;
        unsigned char *entry = object + 0x18;
        do {
            func_089E132C(entry);
            ++i;
            entry += 0x1C;
        } while (i < 4);
    }
    func_089E0DC0(object + 0x88);
    return 0;
}
