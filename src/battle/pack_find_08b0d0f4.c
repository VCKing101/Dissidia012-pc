// Searches the fifth section (count at +0x18, stride 0x24) for an entry whose first short equals the key; returns it or 0.
#include "battle/agent50_types.h"
typedef struct { short key; char pad[0x22]; } Ent08B0D0F4;
Ent08B0D0F4 *func_08B0D0F4(Ref0D188 *ref, int key)
{
    Pack0D188 *p = ref->pack;
    int n0 = p->n0;
    int n4 = p->n4;
    char *a = p->data;
    Ent08B0D0F4 *e = (Ent08B0D0F4 *)(a + n0 * 0x14 + p->n1 * 8 + p->n2 * 0x24 + p->n3 * 0x14);
    Ent08B0D0F4 *end = e + n4;
    for (; e != end; e++) {
        if (e->key == key) {
            return e;
        }
    }
    return 0;
}
