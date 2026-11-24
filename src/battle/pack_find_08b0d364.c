// Searches the first section (count at +8, stride 0x14, from +0x20) for an entry whose first word equals the key; returns it or 0.
#include "battle/agent50_types.h"
typedef struct { int key; char pad[0x10]; } Ent08B0D364;
Ent08B0D364 *func_08B0D364(Ref0D188 *ref, int key)
{
    Pack0D188 *p = ref->pack;
    Ent08B0D364 *e = (Ent08B0D364 *)((char *)p + 0x20);
    Ent08B0D364 *end = e + p->n0;
    for (; e != end; e++) {
        if (e->key == key) {
            return e;
        }
    }
    return 0;
}
