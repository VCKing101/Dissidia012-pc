// Destructor: give back the two reserved blocks to the pool and free the object when asked.
extern "C" struct Pool *D_09B6F478;
extern "C" int func_08971010(int a, int b);
extern "C" void func_08960E5C(void *self);
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
struct H {
    static void tail(int *self, unsigned flags)
    {
        if (self[4] != 0) {
            Pool *g = D_09B6F478;
            int n = func_08971010(g->h18, self[4]);
            g = (Pool *)((char *)g + g->o88->e->slot * 4);
            g->amounts[0] -= n;
        }
        if (flags & 1) {
            func_08960E5C(self);
        }
    }
};
extern "C" void func_0896023C(int *self, unsigned flags)
{
    if (self != 0) {
        if (self[0] != 0) {
            Pool *g = D_09B6F478;
            int n = func_08971010(g->h18, self[0]);
            g = (Pool *)((char *)g + g->o88->e->slot * 4);
            g->amounts[0] -= n;
            self[0] = 0;
        }
        H::tail(self, flags);
    }
}
