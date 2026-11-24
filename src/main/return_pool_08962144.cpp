// Give back two reserved blocks to the pool and subtract their sizes from the owner's budget.
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
extern "C" void func_08962144(int unused, Pool *pool0, int unused2, int *p0)
{
    int *p = p0;
    Pool *pool = pool0;
    if (p[0] != 0) {
        int n = func_08971010(pool->h18, p[0]);
        Pool *g = (Pool *)((char *)pool + pool->o88->e->slot * 4);
        g->amounts[0] -= n;
        p[0] = 0;
    }
    if (p[1] != 0) {
        int n = func_08971010(pool->h18, p[1]);
        pool = (Pool *)((char *)pool + pool->o88->e->slot * 4);
        pool->amounts[0] -= n;
        p[1] = 0;
    }
}
