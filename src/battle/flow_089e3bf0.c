#include "battle/runtime_helpers.h"

/* Connected slot/binding/update flow; full class ownership remains unknown. */


int func_089E3BF0(unsigned char *object)
{
    if (!object[0]) return -1;
    func_089E4A3C(object + 8);
    func_089E4A3C(object + 0x3C);
    func_089E4368(object + 0x70);
    return 0;
}
