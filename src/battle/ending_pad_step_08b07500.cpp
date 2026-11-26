/* Battle ending screen step: handles the replay pad keys (save/cancel prompts), then picks the next page and ticks its timer down. */
struct VEnt { short adj; short pad; int (*fn)(void *, long long); };
struct VT { char pad0[0x10]; VEnt e10; char pad1[8]; VEnt e20; };
struct Pad {
    char pad0[0x34];
    VT *vt;
};
struct Rep { char pad0[0xBDE8]; unsigned flags; };
struct Field { char p0[0x4C]; int mode; char p1[0x740 - 0x50]; int f740; char p2[0x7C8 - 0x744]; Rep *rep; };
struct Timer { char p0[0x18]; float step; };
struct EObj {
    char pad0[0x10];
    int i10;
    char pad1[0x2C - 0x14];
    float time;
};
struct T {
    static bool has(unsigned v, unsigned m) { return (v & m) != 0; }
    static bool eq(int v, int k) { return v == k; }
};
extern "C" {
extern void *D_08B05258;
extern void *D_08B1D820;
extern Field *D_089D6A10;
extern Timer *D_089D69E0;
extern long long D_08B0CDA8;
extern long long D_08B0CDB0;
extern long long D_08B0CDB8;
extern long long D_08B0CDC0;
extern void *func_08842164(void);
extern Pad *func_088729A0(int a);
extern int func_0888B080(void);
extern int func_08B05008(void *p);
extern void func_08B050A8(void *p, int a);
extern void func_08B081D4(EObj *o, int a);
extern void func_08B1D044(void *p);
extern void func_08B1D190(void *p);
extern void func_08B1D1E8(void *p);
extern int func_08B1D2D8(void *p);
extern void func_08B1D828(int a);
extern int func_08B07500(EObj *self) {
    void *pc = func_08842164();
    int one = 1;
    if (pc != 0) {
        Pad *pad = func_088729A0(0);
        if (func_0888B080() == 0) {
            VEnt *e = &pad->vt->e20;
            if (e->fn((char *)pad + e->adj, D_08B0CDA8) == one) {
                func_08B1D190(D_08B1D820);
            }
            e = &pad->vt->e10;
            if (e->fn((char *)pad + e->adj, D_08B0CDB0) == one) {
                e = &pad->vt->e10;
                if (e->fn((char *)pad + e->adj, D_08B0CDB8) == 0) {
                    func_08B1D1E8(D_08B1D820);
                }
            }
        }
        void *p = D_08B05258;
        Pad *pad2 = pad;
        int m = func_08B05008(p);
        VEnt *e = &pad2->vt->e10;
        if (e->fn((char *)pad2 + e->adj, D_08B0CDC0) == one) {
            if (m == 0) {
                func_08B050A8(p, 2);
                func_08B1D828(9);
            } else if (m == one) {
                func_08B1D828(10);
            }
        }
    }
    func_08B1D044(D_08B1D820);
    if (func_08B1D2D8(D_08B1D820) == one && self->time <= 0.0f) {
        if (D_089D6A10->mode == 9 && T::has(D_089D6A10->rep->flags, 2)) {
            func_08B081D4(self, 0x11);
        } else {
            int v = D_089D6A10->f740;
            if (T::eq(v, 1) == one || T::eq(v, 3) == one || T::eq(v, 2) == one) {
                if (self->i10 != 0) {
                    func_08B081D4(self, 0xE);
                } else {
                    func_08B081D4(self, 0x10);
                }
            } else {
                func_08B081D4(self, 0xC);
            }
        }
    }
    self->time = self->time - D_089D69E0->step;
    return 0;
}
}
