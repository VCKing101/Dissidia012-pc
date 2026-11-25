struct V4 { float x, y, z, w; };
struct E { char p[0x80]; V4 v80; char q[0x100 - 0x90]; char h100[0x10]; char r[0x542 - 0x110]; unsigned char b542; };
struct Own { int pad; unsigned int sel; };
struct Info { char p[0x2C]; unsigned char b; };
extern "C" {
void *func_08899300();
int func_08B60238(Own *);
int *func_08B53804();
E *func_08841F64(E *);
int func_08B5FA5C(Own *, V4 *);
void func_089A3868(void *, void *);
void func_088976E4(void *, void *, void *, int, int, int, int);
void func_08B5F994(Own *, V4 *);
}
struct H { static bool z(int v) { return v == 0; } };
extern "C" void func_08B5FDE4(Own *o, Own *tgt) {
    Info info;
    char tmp[0x10];
    V4 a;
    V4 b;
    E *e = 0;
    void *u = func_08899300();
    int n = func_08B60238(o);
    if (n == 0) return;
    int *seed = func_08B53804();
    int nv = *seed * 0x343FD + 0x269EC3;
    n = (nv >> 16 & 0x7FFF) % n;
    *seed = nv;
    while ((e = func_08841F64(e)) != 0) {
        if (H::z(e->b542)) {
            V4 *p = &e->v80;
            a = *p;
            if (func_08B5FA5C(tgt, &a) != 0) {
                func_089A3868(tmp, e->h100);
                func_088976E4(u, &info, tmp, 2, 1, 0, 0);
                if (o->sel == info.b || o->sel + 1 == info.b) {
                    if (n == 0) {
                        b = *p;
                        func_08B5F994(tgt, &b);
                    }
                    n = n - 1;
                }
            }
        }
    }
}
