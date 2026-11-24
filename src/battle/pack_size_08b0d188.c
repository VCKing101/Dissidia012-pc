// Computes the end offset of a packed resource from its section counts (+8..+0x18) and entry sizes (0x14, 8, 0x24, 0x14, 0x24).
#include "battle/agent50_types.h"
char *func_08B0D188(Ref0D188 *ref)
{
    Pack0D188 *p = ref->pack;
    char *a = p->data;
    return a + p->n0 * 0x14 + p->n1 * 8 + p->n2 * 0x24 + p->n3 * 0x14 + p->n4 * 0x24;
}
