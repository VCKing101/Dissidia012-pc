typedef struct W { char pad[0x5A4]; int h5a4; char pad2[0x1C]; int h5c4; } W;
typedef struct G { unsigned char on; char p[3]; int h4; unsigned h8; int hc; int h10; int h14; } G;
extern W *func_08889E44(void);
extern int func_08A642BC(int);
extern unsigned func_08A7EC58(int, int);
extern void func_0887BA5C(int);
extern int func_0887AFBC(int, int, int);
extern int func_0887AD78(int, int, int, int *, int, int);
extern void func_0887B9A4(int);
extern int D_089E1C40[];

int func_089E132C(G *self) {
    W *w;
    int a, b, h, kind, eff;
    unsigned st;
    int arg[1];
    if (self->on == 0) {
        return -1;
    }
    w = func_08889E44();
    if (w == 0) {
        return -2;
    }
    a = w->h5a4;
    if (a == 0) {
        return -3;
    }
    b = w->h5c4;
    if (b == 0) {
        return -4;
    }
    st = func_08A7EC58(func_08A642BC(b), self->hc);
    if (self->h8 != st) {
        h = self->h4;
        if (h != 0) {
            func_0887BA5C(h);
            self->h4 = 0;
        }
        if (st < 0x13) {
            kind = D_089E1C40[st];
            eff = 0;
            if (kind < 6) {
                if (kind >= 5) {
                    eff = func_0887AFBC(self->h14, *(unsigned char *)(a + 0x6A), 0);
                }
            } else if (kind == 0xB) {
                eff = func_0887AFBC(self->h14, *(unsigned char *)(a + 0x6B), 0);
            }
            if (eff == 0) {
                h = self->h4 = func_0887AD78(self->h14, 0, kind, 0, 0, 0);
            } else {
                arg[0] = eff + 2;
                h = self->h4 = func_0887AD78(self->h14, 0, kind, arg, 1, 0);
            }
            if (h != 0) {
                func_0887B9A4(h);
                *(unsigned *)(self->h4 + 0x10) |= 0x8000;
            }
        }
        self->h8 = st;
    }
    return 0;
}
