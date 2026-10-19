#include "battle/runtime_helpers.h"

/* Observed state/update helpers; original gameplay ownership is unresolved. */

int func_089E3C3C(unsigned char *object)
{
    if (!object[0]) return -1;
    func_089E4368(object + 0x70);
    return 0;
}
