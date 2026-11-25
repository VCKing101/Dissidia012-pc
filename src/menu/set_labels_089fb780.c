typedef struct Wk {
    char pad[0xC7C]; int fC7C; int fC80; int fC84;
    char pad2[0x1E78 - 0xC88]; char f1E78[0x80]; char f1EF8[0x80];
    char pad3[0x2478 - 0x1F78]; char f2478[0x80];
    char pad4[0x27B6 - 0x24F8]; short f27B6; int f27B8;
} Wk;
typedef struct Sc { char pad[0x14C]; Wk *wk; } Sc;
extern int D_089D6998;
extern char D_08A0AC54[], D_08A0AC88[];
extern void func_0887BA5C(int);
extern void func_0893476C(char *, char *, int);
extern void func_0887DF14(int, char *, int, char *);
extern int func_089EF1E8(Sc *, int, int, int *, int);
extern int func_089FCD38(Sc *);
extern int func_0887D868(int, int, char *, int, int, int, int, int);
extern int func_0887E534(int);
void func_089FB780(Sc *self)
{
    char buf[64];
    int args[2];
    Wk *w = self->wk;
    if (w->fC7C != 0) {
        func_0887BA5C(w->fC7C);
        self->wk->fC7C = 0;
        w = self->wk;
    }
    func_0893476C(buf, D_08A0AC54, w->f27B6);
    func_0887DF14(D_089D6998, self->wk->f1E78, 0x80, buf);
    args[0] = (int)self->wk->f1E78;
    func_0893476C(buf, D_08A0AC54, self->wk->f27B8);
    func_0887DF14(D_089D6998, self->wk->f1EF8, 0x80, buf);
    args[1] = (int)self->wk->f1EF8;
    self->wk->fC7C = func_089EF1E8(self, 0, 0xD, args, 2);
    w = self->wk;
    if (w->fC80 != 0) {
        func_0887BA5C(w->fC80);
        self->wk->fC80 = 0;
        w = self->wk;
    }
    if (w->fC84 != 0) {
        func_0887BA5C(w->fC84);
        self->wk->fC84 = 0;
    }
    {
        int v = func_089FCD38(self);
        func_0893476C(buf, D_08A0AC88, v);
    }
    func_0887DF14(D_089D6998, self->wk->f2478, 0x80, buf);
    {
        char *lab = self->wk->f2478;
        self->wk->fC80 = func_0887D868(D_089D6998, 0, lab, -1, 0, 0, 0, 0);
    }
    self->wk->fC84 = func_0887E534(self->wk->fC80);
}
