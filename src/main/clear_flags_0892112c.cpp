// When the pending bit is set and both checks pass, clear the pending and the dirty bits of the flag word.
extern "C" int func_08B3486C(int a);
extern "C" int func_088A751C(void);
struct Sub { char pad[4]; int h; char pad2[0x3C]; int v; };
struct Obj { unsigned flags; int a; int b; Sub *sub; };
struct E {
    static bool nz(int c) { return c != 0; }
    static void clr(unsigned *p, unsigned m) { *p = *p & ~m; }
};
extern "C" void func_0892112C(Obj *o)
{
    if (!E::nz(o->flags & 8)) {
        return;
    }
    if (func_08B3486C(o->sub->h) == 0) {
        return;
    }
    if (o->sub->v != 0) {
        if (func_088A751C() != 0) {
            return;
        }
    }
    E::clr(&o->flags, 8);
    E::clr(&o->flags, 2);
}
