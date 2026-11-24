// Give back up to three reserved blocks of the record to the pool and clear the owner link.
extern "C" int func_08971010(int a, int b);
struct Entry { char pad[0x18]; unsigned short slot; };
struct Owner { char pad[0x1C8]; Entry *e; };
struct Pool {
    char pad0[0x18];
    int h18;
    char pad1[0x88 - 0x1C];
    Owner *o88;
    char pad2[0xB4 - 0x8C];
    int amounts[1];
};
extern "C" void func_08966700(int unused, Pool *pool0, int unused2, int **link0)
{
    int **link = link0;
    int *r = *link;
    Pool *pool = pool0;
    if (r != 0) {
        int h = pool->h18;
        if (r[0] != 0) {
            int n = func_08971010(h, r[0]);
            Pool *g = (Pool *)((char *)pool + pool->o88->e->slot * 4);
            g->amounts[0] -= n;
            r[0] = 0;
            h = pool->h18;
        }
        if (r[1] != 0) {
            int n = func_08971010(h, r[1]);
            Pool *g = (Pool *)((char *)pool + pool->o88->e->slot * 4);
            g->amounts[0] -= n;
            r[1] = 0;
            h = pool->h18;
        }
        int n = func_08971010(h, (int)*link);
        pool = (Pool *)((char *)pool + pool->o88->e->slot * 4);
        pool->amounts[0] -= n;
        *link = 0;
    }
}
