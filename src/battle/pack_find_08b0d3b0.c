// Searches the second section (count at +0xC, stride 8, after the first section) for an entry whose first word equals the key; returns it or 0.
#include "battle/agent50_types.h"
typedef struct { int key; int val; } Ent08B0D3B0;
typedef struct { int key; char pad[0x10]; } First08B0D3B0;
Ent08B0D3B0 *func_08B0D3B0(Ref0D188 *ref, int key)
{
    Pack0D188 *p = ref->pack;
    char *base = p->data;
    Ent08B0D3B0 *e = (Ent08B0D3B0 *)(base + p->n0 * 0x14);
    Ent08B0D3B0 *end = e + p->n1;
    for (; e != end; e++) {
        if (e->key == key) {
            return e;
        }
    }
    return 0;
}
