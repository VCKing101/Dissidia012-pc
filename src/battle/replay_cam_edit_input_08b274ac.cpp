/* Replay Camera Edit input handler: reads the pad and sets, removes or normalizes camera data and spans at the play frame. */
struct VEnt { short adj; short pad; int (*fn)(void *, long long); };
struct VT { char pad0[8]; VEnt e8; VEnt e10; };
struct Pad { char pad0[0x34]; VT *vt; };
struct Field { char p0[0x4C]; int mode; };
struct Obj {
    Pad *pad;
    void *rep;
    char p8[8];
    int cam[4];
    char p20[0x1A0 - 0x20];
    unsigned char b1A0;
    char p1A1[8];
    unsigned char b1A9;
};
struct B {
    static int k8(Pad *p, long long m) { VEnt *e = &p->vt->e8; return e->fn((char *)p + e->adj, m); }
    static int k10(Pad *p, long long m) { VEnt *e = &p->vt->e10; return e->fn((char *)p + e->adj, m); }
    static unsigned char busy(int a, int b) { return (b == 1) | (a != 0); }
};
extern "C" {
extern Field *D_089D6A10;
extern float D_089D5EAC;
extern int D_089D6030;
extern unsigned char D_089D603B;
extern void *D_089D6D80;
extern void *D_08B28588;
extern long long D_08B27868;
extern long long D_08B27870;
extern long long D_08B278A0;
extern long long D_08B278C8;
extern long long D_08B278D0;
extern int func_08889B08(Field *f);
extern int func_08889B68(Field *f);
extern int func_0888B0A4(void);
extern void func_088A5F1C(void *a, void *b, void *c);
extern void func_088C9AEC(void *r, void *c);
extern void func_088C9F2C(void *r, void *c);
extern void func_088C9FE8(void *r);
extern void func_088CA4C8(void *r);
extern void func_088CA4EC(void *r);
extern void func_088CA510(void *r);
extern void func_08B234D0(Obj *o, int a);
extern int func_08B2391C(Obj *o);
extern int func_08B239EC(Obj *o);
extern void func_08B26D20(Obj *o);
extern void func_08B27F80(void *p);
void func_08B274AC(Obj *self) {
    Field *f = D_089D6A10;
    int z;
    int r;
    long long m;
    self->b1A9 = 0;
    if (func_08B239EC(self) == 0 || func_08B2391C(self) == 1 || func_0888B0A4() == 1) {
        if (func_08889B08(f) == 1 && D_089D5EAC != 0.0f) {
            func_08B26D20(self);
        }
    } else {
        func_08B234D0(self, 1);
    }
    if (func_08B239EC(self) == 1) {
        z = B::k8(self->pad, D_08B27868);
        while (1) { if (z == 0) {
            if (func_08B2391C(self) != 1 && func_0888B0A4() != 1) {
                r = B::k10(self->pad, D_08B278D0);
                m = D_08B278C8;
                if (r != 0) {
                    func_088C9AEC(self->rep, self->cam);
                }
                if (B::k10(self->pad, m) != 0) {
                    func_088C9FE8(self->rep);
                }
            }
        } else {
            r = B::k10(self->pad, D_08B278D0);
            m = D_08B278C8;
            if (r != 0) {
                func_088CA4C8(self->rep);
            }
            if (B::k10(self->pad, m) != 0) {
                func_088CA4EC(self->rep);
            }
            if (B::k10(self->pad, D_08B27870) != 0) {
                if (B::busy(D_089D6030, D_089D603B) == 0) {
                    D_089D603B = 1;
                    func_08B27F80(D_08B28588);
                }
            }
            if (B::k10(self->pad, D_08B278A0) != 0) {
                func_088C9F2C(self->rep, self->cam);
            }
        }
        break; }
    } else if (func_08889B68(D_089D6A10) == 1) {
        if (B::k10(self->pad, D_08B278C8) == 1) {
            func_088CA510(self->rep);
        }
    }
    if (self->b1A0 == 1) {
        func_088A5F1C(D_089D6D80, (char *)self + 0x40, (char *)self + 0x100);
    }
}
}
