extern "C" void func_0885C93C(void *);
extern "C" void func_08813BE4(void *, void *, int, int);
extern "C" int func_0885CA98(void *, int, int, int);
extern "C" void func_08A801B4(int, int, int);
extern "C" void func_0885AF78(int, int, int);
extern "C" void func_08AC61AC(int, int, int, int);
extern "C" char D_089C6F14[], D_089C6F1C[], D_089C6F24[], D_089C6F2C[];
struct K { char a, b, c, d; };
extern "C" struct K D_089C6F34, D_089C6F3C, D_089C6F44, D_089C6F4C, D_089C6F54;
extern "C" int D_08A80218, D_08AC8EF8;
extern "C" int D_089D4DC8;

struct H { static int keyof(K *k) { int c = k->c, d = k->d, b = k->b, a = k->a; return (c << 16 | d << 24) | (a | b << 8); } };
#define KEY(k) H::keyof(&(k))

extern "C" void func_0881391C(void *self)
{
    struct R { int p0, p1, a, b; } *r;
    func_0885C93C(self);
    func_08813BE4(self, D_089C6F14, 0, 0);
    func_08813BE4(self, D_089C6F1C, 0, 1);
    func_08813BE4(self, D_089C6F24, 0, 2);
    func_08813BE4(self, D_089C6F24, 1, 3);
    func_08813BE4(self, D_089C6F2C, 0, 4);
    r = (struct R *)func_0885CA98(self, 1, KEY(D_089C6F34), 0);
    if (r) func_08A801B4(D_08A80218, r->a, r->b);
    r = (struct R *)func_0885CA98(self, 3, KEY(D_089C6F3C), 0);
    if (r) func_0885AF78(D_089D4DC8 + 0x7B0, r->a, r->b);
    r = (struct R *)func_0885CA98(self, 3, KEY(D_089C6F44), 0);
    if (r) func_08AC61AC(D_08AC8EF8, 1, r->a, r->b);
    r = (struct R *)func_0885CA98(self, 3, KEY(D_089C6F4C), 0);
    if (r) func_08AC61AC(D_08AC8EF8, 0, r->a, r->b);
    r = (struct R *)func_0885CA98(self, 3, KEY(D_089C6F54), 0);
    if (r) func_08AC61AC(D_08AC8EF8, 2, r->a, r->b);
}
