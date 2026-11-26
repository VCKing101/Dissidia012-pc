/* Battle ending screen update: Save Replay request (square pad or flag) and poll, circle cancel, advance. */
struct Pad;
struct VEnt { short adj; short pad; int (*fn)(void *, long long); };
struct VT { char pad0[0x10]; VEnt e10; };
struct Pad {
    char pad0[0x34];
    VT *vt;
    inline int pressed(long long a) {
        VEnt *e = &vt->e10;
        return e->fn((char *)this + e->adj, a);
    }
};
struct Pc { char pad0[0x5A0]; Pad *pad; };
struct EObj {
    char pad0[0x3888];
    unsigned char f3888;
    unsigned char f3889;
};
extern "C" {
extern void *D_08B05258;
extern int D_089D7260;
extern unsigned char *D_089D7280;
extern long long D_08B0CDC0;
extern Pc *func_08842164(void);
extern int func_0888B014(void);
extern int func_088AF29C(int a);
extern int func_08B05008(void *p);
extern void func_08B050A8(void *p, int a);
extern void func_08B05164(void *p);
extern void func_0888AD50(void);
extern void func_08B081D4(EObj *o, int a);
extern int func_08B0AA58(EObj *o);
extern void func_08B1D828(int a);
}

extern "C" int func_08B07B94(EObj *self)
{
    Pc *pc = func_08842164();
    void *p = D_08B05258;
    int busy;
    int st;
    Pad *pad;
    VEnt *e;
    if (self->f3889 == 1) {
        if (func_0888B014() == 1) {
            self->f3889 = 0;
            if (func_088AF29C(D_089D7260) == 1) {
                if (func_08B05008(p) != 3) {
                    func_08B050A8(p, 3);
                }
            }
        }
    }
    busy = 0;
    st = func_08B05008(p);
    if (pc && self->f3889 == 0 && (st == 0 || st == 2 || st == 3)) {
        pad = pc->pad;
        if (self->f3888 == 1 || (pad && (e = &pad->vt->e10, e->fn((char *)pad + e->adj, D_08B0CDC0)) == 1)) {
            if (*D_089D7280 == 0) {
                func_0888AD50();
                self->f3888 = 0;
                self->f3889 = 1;
            } else {
                busy = 1;
            }
        }
    }
    if (pc && st == 1) {
        Pad *pc2 = pc->pad;
        if (pc2) {
            if ((e = &pc2->vt->e10, e->fn((char *)pc2 + e->adj, D_08B0CDC0)) == 1) {
                func_08B1D828(10);
            }
        }
    }
    if ((self->f3889 | busy) == 0) {
        if (func_08B0AA58(self) == 1) {
            func_08B081D4(self, 0x11);
            if (func_08B05008(p) < 4u) {
                func_08B05164(p);
            }
        }
    }
    return 0;
}
