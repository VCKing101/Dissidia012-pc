/* Replay save/load prompt step: a small state machine that waits for pad confirm or cancel, then closes the prompt and restores the previous mode. */
struct VEnt { short adj; short pad; int (*fn)(void *, long long); };
struct VT { char pad0[8]; VEnt e8; VEnt e10; };
struct Pad {
    char pad0[0x34];
    VT *vt;
};
struct Field { char p0[0x48]; int m48; };
struct Time { char p0[0x18]; float step; };
struct Menu {
    int obj;
    int mode;
    int state;
    float t;
    float limit;
    unsigned char flag;
    unsigned char active;
};
struct GC { char p0[0x727C]; int w; };
extern "C" int func_088B1440(int a);
extern "C" int D_089D727C;
struct GB { char p0[0x6A10]; Field *f; char p1[0x7280 - 0x6A14]; char *q; };
struct B {
    static bool is(int v, int k) { return v == k; }
    static int k8(Pad *p, long long m) { VEnt *e = &p->vt->e8; return e->fn((char *)p + e->adj, m); }
    static bool okc(GB *x)
    {
        if (*(unsigned char *)x->q != 0) {
            return true;
        }
        int v = func_088B1440(D_089D727C);
        return v != 0 && v != 0x80110005;
    }
    static int k10(Pad *p, long long m) { VEnt *e = &p->vt->e10; return e->fn((char *)p + e->adj, m); }
};
extern "C" {
extern int D_089D727C;
extern int func_088B1440(int a);
extern void func_088B1764(char *a);
extern void func_08B2CE28(Menu *m);
extern Field *D_089D6A10;
extern Time *D_089D69E0;
extern int D_089D6AE8;
extern char D_089D6C98;
extern unsigned char D_089D5F1D;
extern char D_089D0000[];
extern char *D_089D7280;
extern long long D_08B2D298;
extern long long D_08B2D2A0;
extern long long D_08B2D2A8;
extern long long D_08B2D2B0;
extern Pad *func_088720A4(void);
extern void func_08889944(Field *f, int v);
extern void func_0888A8EC(void);
extern void func_0889661C(int a);
extern void func_088A2780(void);
extern void func_088A2874(float a, float b);
extern void func_088C99E4(int a);
extern void func_088C9A54(int a);
extern int func_088C9AA4(int a);
extern int func_088CE080(int a);
extern int func_088CE2A8(void);
extern void func_08B2CEB4(Menu *m);
int func_08B2CF9C(Menu *self) {
    if (self->active == 0) {
        return -1;
    }
    GB *x;
    switch (self->state) {
    case 1:
        self->state = 2;
        x = (GB *)0x089D0000;
        goto common;
    case 2:
        x = (GB *)0x089D0000;
    common: {
        Pad *a = func_088720A4();
        if (B::is(x->f->m48, 4)) {
            break;
        }
        if (func_088C9AA4(self->obj) < 0) {
            func_088C9A54(self->obj);
            func_08889944(x->f, 0x40);
            func_08B2CEB4(self);
            self->state = 4;
        } else {
            VEnt *e = &a->vt->e8;
            if (e->fn((char *)a + e->adj, D_08B2D298) != 0) {
                e = &a->vt->e8;
                if (e->fn((char *)a + e->adj, D_08B2D2A0) != 0) {
                    e = &a->vt->e10;
                    int r;
                    if (D_089D5F1D != 0) {
                        r = e->fn((char *)a + e->adj, D_08B2D2A8);
                    } else {
                        r = e->fn((char *)a + e->adj, D_08B2D2B0);
                    }
                    if (r != 0) {
                        func_088C9A54(self->obj);
                        func_08889944(x->f, 0x40);
                        func_08B2CE28(self);
                        self->state = 3;
                    }
                }
            }
        }
        break;
    }
    case 3: {
        char z0 = 0;
        float t = self->t + D_089D69E0->step;
        self->t = t;
        if (!(t < self->limit)) {
            func_088C99E4(self->obj);
            func_0888A8EC();
            func_08889944(D_089D6A10, self->mode);
            func_088A2780();
            func_0889661C(D_089D6AE8);
            func_088A2874(0, 0);
            D_089D6C98 = z0;
            self->state = 0;
        }
        break;
    }
    case 4: {
        x = (GB *)0x089D0000;
        bool ok = B::okc(x);
        if (!ok) {
            func_088B1764(x->q);
            func_08B2CE28(self);
            self->state = 3;
        }
        break;
    }
    case 0:
        break;
    }
    if (self->flag != 0) {
        func_088CE080(func_088CE2A8());
    }
    return 0;
}
}
