// Destructor: reset the vtable, give back the reserved amount to the pool, release and free when asked.
extern "C" char D_089DC4C8[];
extern "C" struct Pool *D_09B6F478;
extern "C" int func_08971010(int a, int b);
extern "C" void func_08969FAC(void *self, int v);
extern "C" void func_08968C64(void *self);
struct Cls { int h; };
struct Info { char pad[0x1C8]; };
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
struct Obj {
    char pad[0x20];
    void *vt;
    int res;
};
extern "C" void func_0896859C(Obj *self, unsigned flags)
{
    if (self != 0) {
        self->vt = D_089DC4C8;
        if (self->res != 0) {
            Pool *g = D_09B6F478;
            int n = func_08971010(g->h18, self->res);
            g = (Pool *)((char *)g + g->o88->e->slot * 4);
            g->amounts[0] -= n;
            self->res = 0;
        }
        func_08969FAC(self, 0);
        if (flags & 1) {
            func_08968C64(self);
        }
    }
}
