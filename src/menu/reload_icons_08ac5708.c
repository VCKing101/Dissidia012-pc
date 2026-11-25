typedef struct Sub { char pad[0x48]; int fA; int fB; char pad2[0x58 - 0x50]; int fC; int fE0; int fE4; int fE8; } Sub;
typedef struct Wk {
    char pad0[0x400]; char f400[0x484]; Sub s;
    char pad8EC[0x8F0 - 0x8EC - 0x0]; char f8F0[0x90]; int f980;
} Wk;
typedef struct Sc { char pad[0x138]; Wk *wk; } Sc;
typedef struct Info { char pad[4]; unsigned short s; short pad2; int pad3[3]; } Info;
extern int D_089D6998;
extern short D_089D7F14;
extern char D_08AC744C[], D_08AC7338[];
extern void func_0887BA5C(int);
extern void func_089347E8(char *, char *, int);
extern int func_0887D868(int, int, char *, int, int, int, int, int);
extern int func_0887E534(int);
extern void func_088F1810(Info *);
extern void func_088F18B8(Info *);
extern int func_088F14A4(void);
extern int func_08AC8A94(int, int, int, int);
extern void func_088D428C(char *, char *, char *, int);
void func_08AC5708(Sc *self)
{
    Wk *w;
    Sub *p;
    Info info;
    int h;
    int n;
    w = self->wk;
    p = &w->s;
    if (p->fA != 0) {
        func_0887BA5C(p->fA);
        p->fA = 0;
    }
    if (p->fB != 0) {
        func_0887BA5C(p->fB);
        p->fB = 0;
    }
    if (p->fC != 0) {
        func_0887BA5C(p->fC);
        p->fC = 0;
    }
    if (p->fE4 != 0) {
        func_0887BA5C(p->fE4);
        p->fE4 = 0;
    }
    if (p->fE0 != 0) {
        func_0887BA5C(p->fE0);
        p->fE0 = 0;
    }
    if (p->fE8 != 0) {
        func_0887BA5C(p->fE8);
        p->fE8 = 0;
    }
    func_089347E8(self->wk->f8F0, D_08AC744C, D_089D7F14);
    p->fA = func_0887D868(D_089D6998, 0, self->wk->f8F0, -1, 0, 0, 0, 0);
    h = p->fA;
    p->fB = func_0887E534(h);
    func_088F1810(&info);
    if (info.s != 0xFFFF) {
        h = func_08AC8A94(self->wk->f980, info.s, 0, 0);
        p->fC = h;
        p->fE4 = func_0887E534(h);
    }
    n = func_088F14A4();
    if (n > 1) {
        func_088F18B8(&info);
        if (info.s != 0xFFFF) {
            h = func_08AC8A94(self->wk->f980, info.s, 0, 0);
            p->fE0 = h;
            p->fE8 = func_0887E534(h);
        }
    }
    func_088D428C(self->wk->f400, (char *)p, D_08AC7338, 1);
}
