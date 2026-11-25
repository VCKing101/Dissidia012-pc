struct Ent { int pad[2]; int a; int b; };
struct Hdr { int pad; void *h; };
extern "C" {
struct K4 { char a, b, c, d; };
extern K4 D_089C6ED4, D_089C6EDC, D_089C6EE4, D_089C6EEC, D_089C6EF4, D_089C6EFC, D_089C6F04, D_089C6F0C;
extern void *D_089D48D0;
extern int D_089D4D80;
Hdr *func_08869AB4(void *p, void *name);
void func_0885C93C(void *h);
Ent *func_0885CA98(void *h, int one, int key, int zero);
void func_0880BE34(int a, int b);
void func_0884E5C0(int a, int b);
void func_08830C74(int a, int b);
void func_0884A944(int a, int b);
void func_08811598(int a, int b);
void func_08811A58(int a, int b);
void func_0884C588(int s, int a, int b);
void func_08850AB8(int a, int b);
}
struct K {
    static int Key(K4 &k) { int hi = k.c << 16 | k.d << 24; int lo = k.a | k.b << 8; return hi | lo; }
};
extern "C" void func_088135D4(void *p)
{
    Hdr *r;
    if (p == 0) return;
    r = func_08869AB4(p, D_089D48D0);
    if (r == 0) return;
    void *h = r->h;
    func_0885C93C(h);
    Ent *e;
    e = func_0885CA98(h, 1, K::Key(D_089C6ED4), 0);
    func_0880BE34(e->a, e->b);
    e = func_0885CA98(h, 1, K::Key(D_089C6EDC), 0);
    func_0884E5C0(e->a, e->b);
    e = func_0885CA98(h, 1, K::Key(D_089C6EE4), 0);
    func_08830C74(e->a, e->b);
    e = func_0885CA98(h, 1, K::Key(D_089C6EEC), 0);
    func_0884A944(e->a, e->b);
    e = func_0885CA98(h, 1, K::Key(D_089C6EF4), 0);
    func_08811598(e->a, e->b);
    e = func_0885CA98(h, 1, K::Key(D_089C6EFC), 0);
    func_08811A58(e->a, e->b);
    e = func_0885CA98(h, 1, K::Key(D_089C6F04), 0);
    if (e != 0) {
        func_0884C588(D_089D4D80, e->a, e->b);
    }
    e = func_0885CA98(h, 1, K::Key(D_089C6F0C), 0);
    func_08850AB8(e->a, e->b);
}
