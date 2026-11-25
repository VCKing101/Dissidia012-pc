typedef struct Hd { char pad[0x10]; unsigned int flags; } Hd;
typedef struct Sub { char pad[8]; int a; Hd *b; } Sub;
typedef struct Wk {
    char f0[0x400]; char f400[0x18]; int f418;
    char pad41C[0x8AC - 0x41C]; char f8AC[0x48]; Sub s8F4;
    char pad904[0x9C8 - 0x904]; char f9C8[0xA58 - 0x9C8]; unsigned int fA58; int fA5C; char padA60[4]; int fA64; int fA68;
    char padA6C[0xB48 - 0xA6C]; unsigned char fB48; unsigned char fB49;
} Wk;
typedef struct Sc { char pad[0x14C]; Wk *wk; } Sc;
extern char D_08AC3D3E[], D_08AC3D7E[], D_08AC4076[], D_08AC4242[];
extern void func_08ABC96C(Sc *, char *);
extern void func_088D3CF0(void *, void *, char *);
extern void func_08ABE47C(Sc *);
extern void func_088D9F5C(int, int, int);
extern void func_0887BA5C(int);
extern int func_08ABBC4C(Sc *, int, int, int, int);
extern Hd *func_0887E534(int);
extern void func_088D428C(void *, void *, char *, int);
extern void func_08ABC8C4(Sc *);
extern void func_08ABFC04(Sc *);
extern void func_088D46E4(void *, int, int, int, int, int);
extern void func_08ABC984(Sc *, char *);
extern void func_08ABF334(Sc *);
void func_08ABE18C(Sc *self)
{
    Wk *w;
    char *s0;
    Sub *p;
    int v;
    Hd *h;
    func_08ABC96C(self, self->wk->f9C8);
    self->wk->fB48 = 1;
    self->wk->fB49 = 1;
    w = self->wk;
    s0 = w->f400;
    p = &w->s8F4;
    func_088D3CF0(w, s0, D_08AC3D3E);
    func_088D3CF0(self->wk, s0, D_08AC3D7E);
    func_088D3CF0(self->wk, s0, D_08AC4076);
    func_08ABE47C(self);
    func_088D9F5C(self->wk->f418, 4, 1);
    if (p->a != 0) {
        func_0887BA5C(p->a);
        p->a = 0;
    }
    if ((int)p->b != 0) {
        func_0887BA5C((int)p->b);
        p->b = 0;
    }
    p->a = func_08ABBC4C(self, 0, 7, 0, 0);
    h = func_0887E534(p->a);
    p->b = h;
    h->flags = h->flags | 0x800;
    func_088D428C(s0, p, D_08AC4242, 1);
    func_08ABC8C4(self);
    func_08ABFC04(self);
    v = self->wk->fA68;
    if (v < 0)
        v = 0;
    func_088D46E4(self->wk->f8AC, v, 0, 7, 0, -1);
    self->wk->fA64 = 8;
    func_08ABC984(self, D_08AC4076);
    func_08ABF334(self);
    self->wk->fA58 = self->wk->fA58 | 0x20;
    self->wk->fA5C = 6;
}
