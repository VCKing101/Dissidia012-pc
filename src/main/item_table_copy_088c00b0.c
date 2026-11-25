/* copy a table of short lists from a source record into the object's inline table. */
typedef struct Item { short a; short n; short *v; } Item;
typedef struct Src { short pad; short cnt; Item *items; } Src;
typedef struct Out { short a; short n; short v[4]; } Out;
typedef struct Dst { Out o[4]; char pad[0x30 - 48]; int count; } Dst;
void func_088C00B0(Dst *d, Src *s)
{
    int i;
    d->count = s->cnt;
    for (i = 0; i < d->count; i++) {
        Item *it = &s->items[i];
        Out *o = &d->o[i];
        int j;
        o->a = it->a;
        o->n = it->n;
        for (j = 0; j < o->n; j++)
            o->v[j] = it->v[j];
    }
}
