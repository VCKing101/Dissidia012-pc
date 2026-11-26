/* Replay pause toggle: while paused, the pad keys resume play (restoring the saved mode); while playing, a key or a camera change pauses it. */
struct VEnt { short adj; short pad; int (*fn)(void *, long long); };
struct VT { char pad0[8]; VEnt e8; VEnt e10; char pad1[8]; VEnt e20; };
struct Pad {
    char pad0[0x34];
    VT *vt;
};
struct Field { char p0[0x48]; int m48; };
struct Pause {
    Pad *pad;
    int cam;
    char p8[0x198 - 8];
    int saved;
    unsigned char paused;
    unsigned char wasPaused;
    unsigned char resumed;
    unsigned char cleared;
    char p1a0[4];
    int last;
};
struct B {
    static int k8(Pad *p, long long m) { VEnt *e = &p->vt->e8; return e->fn((char *)p + e->adj, m); }
    static int k10(Pad *p, long long m) { VEnt *e = &p->vt->e10; return e->fn((char *)p + e->adj, m); }
    static int k20(Pad *p, long long m) { VEnt *e = &p->vt->e20; return e->fn((char *)p + e->adj, m); }
    static bool is(int v, int k) { return v == k; }
};
extern "C" {
extern Field *D_089D6A10;
extern char D_089D6039;
extern long long D_08B27918;
extern long long D_08B278A0;
extern long long D_08B27868;
extern int func_08889B08(Field *f);
extern int func_08889B34(Field *f);
extern void func_08889944(Field *f, int v);
extern int func_088CA42C(int a);
extern void func_08B23060(Pause *p);
void func_08B2721C(Pause *self) {
    Field *f = D_089D6A10;
    self->wasPaused = self->paused;
    int m = func_08889B08(f);
    int start;
    int call;
    if (!(m == 1 || m == 2 || m == 0x20)) {
        return;
    }
    {
        if (self->paused == 0) {
            if (func_08889B34(f) == 1) return;
            int mode = D_089D6A10->m48;
            if (!(B::is(mode, 2) == 1 || B::is(mode, 3) == 1)) return;
{
                start = 0;
                call = 1;
                if (self->resumed != 0) {
                    self->resumed = 0;
                    call = 0;
                    start = 1;
                } else if (B::k10(self->pad, D_08B27918) != 0) {
                    start = 1;
                }
                if (self->cleared == 1) {
                    int k = self->last;
                    if (k != func_088CA42C(self->cam)) {
                        start = 1;
                    }
                }
                if (start != 0) {
                    if (call != 0) {
                        func_08B23060(self);
                    }
                    unsigned char b = 1;
                    self->paused = b;
                    self->cleared = 0;
                    self->saved = func_08889B08(f);
                    func_08889944(f, 0x20);
                    D_089D6039 = b;
                }
}
        } else {
            if (B::k20(self->pad, D_08B278A0) != 0 && B::k8(self->pad, D_08B27868) == 0) {
                func_08889944(f, self->saved);
                self->paused = 0;
                self->saved = 0;
                self->resumed = 1;
            } else if (B::k10(self->pad, D_08B27918) != 0) {
                func_08889944(f, self->saved);
                self->paused = 0;
                self->saved = 0;
                D_089D6039 = 0;
            }
        }
    }
}
}
