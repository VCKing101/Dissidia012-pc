typedef struct Cell { int a; int b; int c; int d; int e; int f; } Cell;
typedef struct Blk {
    char pad[0x5A0]; int w5A0; int w5A4; int w5A8; int w5AC; int w5B0; int w5B4; char pad5B8[4]; int w5BC; int w5C0; int w5C4; int w5C8; char pad5CC[4]; int w5D0;
    unsigned short us5D4[40]; unsigned short us624[40]; char pad674[0x674 - 0x674]; Cell cells[8];
} Blk;
typedef struct Wk { char pad[0x850]; Blk *b; } Wk;
extern void *D_08B01CF8;
extern int func_08AF4054(void *);
extern int func_08AF40C4(void *);
extern void func_08AE2D3C(Wk *);
extern void func_08AE021C(Wk *);
extern int func_08AF9FD4(void *);
extern void func_08AE283C(Wk *);
extern void func_08AF3638(void *, int);
void func_08ADFDC0(Wk *self)
{
    int i;
    int r;
    unsigned int f;
    self->b->w5A4 = 0;
    self->b->w5A0 = 0;
    self->b->w5A8 = 5;
    self->b->w5BC = 0;
    self->b->w5AC = 1;
    self->b->w5B0 = 0;
    self->b->w5B4 = 0;
    self->b->w5C4 = 0;
    self->b->w5C8 = 3;
    self->b->w5C0 = 0;
    self->b->w5D0 = 0;
    for (i = 0; i < 0x28; i++) {
        self->b->us5D4[i] = 0;
        self->b->us624[i] = 0;
    }
    for (i = 0; i < 8; i++) {
        self->b->cells[i].a = 0;
        self->b->cells[i].f = 0;
        self->b->cells[i].d = 0;
        self->b->cells[i].b = 0;
        self->b->cells[i].c = 0;
        self->b->cells[i].e = 0;
    }
    r = func_08AF4054(D_08B01CF8);
    if (r == 0) {
        self->b->w5A4 = self->b->w5A4 | 8;
    }
    r = func_08AF40C4(D_08B01CF8);
    if (r == 0) {
        self->b->w5A4 = self->b->w5A4 | 0x20;
    }
    func_08AE2D3C(self);
    func_08AE021C(self);
    r = func_08AF9FD4(D_08B01CF8);
    f = self->b->w5A4;
    if (r == 1) {
        self->b->w5A4 = f | 0x10;
        f = self->b->w5A4;
    }
    if ((f & 8) != 0) {
        func_08AE283C(self);
    }
    func_08AF3638(D_08B01CF8, 0xC72);
}
