typedef struct Wk {
    char pad0[4]; char f4[0x880]; int f884;
    char pad888[0x97C - 0x888]; char *f97C; unsigned char f980; char pad981; short f982;
    int f984; int f988; int f98C; int f990; int f994; int f998; int f99C;
    short f9A0; short f9A2; short f9A4; short f9A6; short f9A8;
    char pad9AA[0x9C4 - 0x9AA]; char f9C4[0x7C10 - 0x9C4]; char f7C10[0x224];
    char f7E34[0x1A]; char pad7E4E[0x8040 - 0x7E4E]; int f8040; int f8044;
    char pad8048[0x81B4 - 0x8048]; unsigned int f81B4; char pad81B8[0x8AB0 - 0x81B8]; char f8AB0[1];
} Wk;
typedef struct Sc { char pad[0x138]; int pool; char pad2[0x154-0x13C]; Wk *wk; } Sc;
typedef struct G { char pad[8]; char f8[0x2EC]; unsigned int f2F4; } G;
extern G D_09B6EFB0;
extern int func_0885C6CC(int, int, int, int, int, int);
extern void func_08AAB25C(int);
extern void func_08AAB31C(int, int, char *, char *, char *, char *);
extern void func_08935524(char *, char *, int);
extern void func_08AAFC68(int, int, char *, char *, char *, char *, int, char *);
void func_08AA1420(Sc *self)
{
    Wk *w;
    int r;
    int o;
    int one = 1;
    if (self->wk->f8044 != 0)
        return;
    {
        self->wk->f97C = self->wk->f4;
        self->wk->f980 = 0x1D;
        self->wk->f982 = 0;
        self->wk->f984 = self->wk->f884;
        self->wk->f988 = 0;
        self->wk->f98C = 3;
        self->wk->f990 = 1;
        self->wk->f994 = 9;
        self->wk->f998 = 0xC;
        self->wk->f99C = 0xB;
        self->wk->f9A0 = 0x708B;
        self->wk->f9A2 = 0x700C;
        self->wk->f9A4 = 0x704F;
        self->wk->f9A6 = 0x70B2;
        self->wk->f9A8 = 0x70D2;
        self->wk->f8040 = func_0885C6CC(self->pool, 0x7D04, 0, 0, one, 0);
        w = self->wk;
        r = w->f8040;
        o = 0;
        if (r != 0) {
            func_08AAB25C(r);
            o = r;
            w = self->wk;
        }
        w->f8044 = o;
        w = self->wk;
        if ((w->f81B4 & 0x40) == 0) {
            func_08AAB31C(w->f8044, self->pool, (char *)&w->f97C, w->f7C10, w->f9C4, w->f8AB0);
        } else {
            func_08935524(w->f7E34, D_09B6EFB0.f8, 0x1A);
            w = self->wk;
            func_08AAFC68(w->f8044, self->pool, (char *)&w->f97C, w->f7C10, w->f9C4, w->f8AB0, 1, (char *)self + 0x894);
            D_09B6EFB0.f2F4 = D_09B6EFB0.f2F4 & 0xFFFFFFFD;
        }
    }
}
