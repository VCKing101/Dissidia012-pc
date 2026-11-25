// If the +0x2A44 flag word has bit 2 set, runs the three step calls until one reports success; otherwise runs the two reset calls.
struct S { char p[0x2A44]; unsigned int flags; };
struct H {
    static bool has(unsigned int v, unsigned int m) { return (v & m) != 0; }
};
struct G { char p[0x4C]; int a; char q[0x6F0]; int b; };
extern G *D_089D6A10;
extern "C" int func_0891A44C(void *p);
extern "C" int func_0891C1A4(void *self, int v);
extern "C" int func_0891C1FC(void *self, int v);
extern "C" void func_0891C330(void *self);
extern "C" void func_0888964C(G *g, int a, int b, int c, int d);
extern "C" void func_08889808(G *g);
extern "C" void func_0891C55C(S *self) {
    if (H::has(self->flags, 4) == 1) {
        int v = func_0891A44C((char *)self + 0x27E0);
        if (func_0891C1A4(self, v) == 0) {
            if (func_0891C1FC(self, 0) == 0) {
                func_0891C330(self);
            }
        }
    } else {
        G *g = D_089D6A10;
        func_0888964C(g, g->a, 0, g->b, 0);
        func_08889808(g);
    }
}
