extern "C" {
    typedef struct Pr { int h; int g; } Pr;
    typedef struct E { char p0[8]; int h8; int hc; Pr pr[7]; } E;
    typedef struct B4 { char p0[0x14]; int h418_; int h41c; int h420; } B4;
    typedef struct W {
        char p0[0x400]; int a400[8]; char p1[0x8ac - 0x420]; char a8ac[0x48]; E e; char p2[0xa58 - 0x93c];
        unsigned fa58; int a5c_; int p5; int a64; char p7[0xa74 - 0xa68]; int a74; int a78; char p8[0xb51 - 0xa7c]; char b51;
    } W;
    typedef struct Obj { char p0[0x14C]; W *w; } Obj;
    extern unsigned short D_08AC3EFE[];
    extern unsigned short D_08AC42B6[];
    extern char D_08AC3D3E[], D_08AC3D7E[], D_08AC4242[], D_08AC425A[];
    extern void func_08ABC96C(Obj *, void *);
    extern void func_088D3CF0(void *, void *, void *);
    extern void func_08ABE438(Obj *, int);
    extern void func_088D9F5C(int, int, int);
    extern void func_0887BA5C(int);
    extern int func_08ABBC4C(Obj *, int, int, int, int);
    extern int func_0887E534(int);
    extern void func_088D3AE8(void *, int, int, int);
    extern void func_088D428C(void *, void *, void *, int);
    extern void func_08ABC8C4(Obj *);
    extern void func_088D46E4(void *, int, int, int, int, int);
    extern void func_08ABC984(Obj *, void *);
    extern int func_08ABC868(Obj *);
    extern int func_08ABC934(Obj *, int);
    void func_08ABE4D4(Obj *self) {
        W *w;
        int *b;
        E *e;
        int i;
        Pr *p;
        unsigned short *tb;
        func_08ABC96C(self, (char *)self->w + 0x9c8);
        w = self->w;
        b = w->a400;
        e = &w->e;
        func_088D3CF0(w, b, D_08AC3D3E);
        func_088D3CF0(self->w, b, D_08AC3D7E);
        func_088D3CF0(self->w, b, D_08AC3EFE);
        func_08ABE438(self, 2);
        func_088D9F5C(*(int *)((char *)self->w + 0x418), 4, 1);
        if (e->h8 != 0) {
            func_0887BA5C(e->h8);
            e->h8 = 0;
        }
        if (e->hc != 0) {
            func_0887BA5C(e->hc);
            e->hc = 0;
        }
        e->h8 = func_08ABBC4C(self, 0, 6, 0, 0);
        e->hc = func_0887E534(e->h8);
        *(unsigned *)(e->hc + 0x10) |= 0x800;
        self->w->a78 = func_08ABC934(self, self->w->a74);
        func_088D9F5C(b[8], 0x1a, 1);
        func_088D3AE8(b, 0x11, *(unsigned short *)(self->w->a78 * 8 + (int)D_08AC3EFE + 6), 8);
        func_088D428C(b, e, D_08AC4242, 1);
        func_08ABC8C4(self);
        i = 4;
        tb = D_08AC42B6;
        for (p = e->pr; i < 0x12; i += 2, p++) {
            int u = (int)((unsigned)((i - 4) >> 1) >> 31);
            p->h = func_08ABBC4C(self, 0, D_08AC42B6[((i + u) - 4 >> 1) * 4 + 1], 0, 0);
            p->g = func_0887E534(p->h);
        }
        func_088D428C(b, e, D_08AC425A, 1);
        func_088D46E4(self->w->a8ac, self->w->a78, 0, 6, 0, -1);
        self->w->a64 = 7;
        func_08ABC984(self, D_08AC3EFE);
        self->w->fa58 |= 0x20;
        self->w->a5c_ = func_08ABC868(self);
        self->w->b51 = 0;
    }
}
