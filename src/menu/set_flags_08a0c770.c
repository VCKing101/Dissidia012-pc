typedef struct V3 { int a; int b; int c; } V3;
typedef struct Wk {
    char pad0[0xFF0]; V3 fFF0;
    char padFFC[0x1070 - 0xFFC]; int f1070;
    char pad1074[0x1140 - 0x1074]; int f1140; unsigned char f1144;
} Wk;
typedef struct Sc { char pad[0x1320]; Wk *wk; } Sc;
typedef struct Bs { char pad[0x3BC4]; short t[1]; } Bs;
typedef struct Rec { unsigned short f0; short pad2; V3 v; char pad10[0xD4 - 0x10]; unsigned int fD4; } Rec;
extern int D_08A23FF0;
extern void func_08A0C72C(Sc *);
extern int func_08A1D350(int);
void func_08A0C770(Sc *self)
{
    char *base;
    int idx;
    Rec *rec;
    unsigned short fl;
    base = *(char **)(D_08A23FF0 + 0x154) + 0x2BB8;
    idx = ((Bs *)base)->t[self->wk->f1070];
    rec = (Rec *)(base + idx * 0x24C + 8);
    func_08A0C72C(self);
    self->wk->f1140 = idx;
    self->wk->fFF0 = rec->v;
    self->wk->f1144 = 0;
    self->wk->f1144 = self->wk->f1144 | 1;
    self->wk->f1144 = self->wk->f1144 | 2;
    fl = rec->f0;
    if ((fl & 2) != 0) {
        self->wk->f1144 = self->wk->f1144 | 0x2D;
    } else {
        if ((fl & 8) != 0) {
            self->wk->f1144 = self->wk->f1144 | 0x20;
            self->wk->f1144 = self->wk->f1144 & ~1;
            self->wk->f1144 = self->wk->f1144 & ~2;
        }
    }
    if (func_08A1D350(D_08A23FF0) != 0)
        self->wk->f1144 = self->wk->f1144 | 4;
    if ((rec->f0 & 4) != 0)
        self->wk->f1144 = self->wk->f1144 | 0x40;
    if ((rec->fD4 & 1) != 0)
        self->wk->f1144 = self->wk->f1144 | 0x80;
}
