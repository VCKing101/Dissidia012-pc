typedef struct Self98F00 { char pad[0x154]; int sub; } Self98F00;
typedef struct In98F00 { char pad[0x12C]; unsigned f3e0; char pad2[0x15C - 0x130]; int f410; } In98F00;
typedef struct G98F00 { int a; int b; char pad[0x304 - 8]; In98F00 in; } G98F00;
typedef struct Ent98F00 { char pad[0xF4]; char f[1]; } Ent98F00;
extern G98F00 D_09B6EFB0;
extern int D_089D712C;
extern char D_08AA3BE0[];
extern void func_08AB58E0(int, int, int);
extern void func_08AB5790(int, int);
extern void func_08A9C79C(void *, int);
extern int func_088F51DC(int);
extern void func_08935828(int, int, int);
extern void func_08935524(int, int, int);
extern void func_088D3A04(int);
extern void func_088D3910(int, int, void *, int, int);
extern void func_08AB3908(int, int);
extern void func_08AA1420(void *);
extern void func_08A9CF8C(void *);
extern void func_08A9C55C(void *, int);
extern void func_08AAB960(int);
extern void func_08A9F190(void *);
extern void func_08A9BEA0(void *);
#define SB(o) (*(int *)(self->sub + (o)))
#define SBU(o) (*(unsigned *)(self->sub + (o)))

void func_08A98F00(Self98F00 *self)
{
    int a;
    int r;
    a = self->sub;
    if ((*(unsigned *)(a + 0x81B4) & 1) != 0) {
        a = (int)self + 0x894;
        func_08AB58E0(a, 6, 1);
        func_08AB58E0(a, 9, 5);
        func_08AB5790(a, 2);
        a = self->sub;
        if ((*(unsigned *)(a + 0x81B4) & 0x40) != 0) {
            {
                G98F00 *g = &D_09B6EFB0;
                In98F00 *i = &g->in;
                func_08A9C79C(self, i->f410);
            }
            a = self->sub;
        }
    }
    *(short *)(a + 0x8AC0) = 0;
    if (D_09B6EFB0.a == 2) {
        {
            Ent98F00 *pe = (Ent98F00 *)(D_089D712C + 0x1FDD8);
            char *pf = pe->f;
            r = func_088F51DC((int)(pf + D_09B6EFB0.b * 0x10A8));
        }
        if (r != 0) {
            func_08935828(self->sub + 0x8AC0, 0, 0x1C);
            func_08935524(self->sub + 0x8AC0, r + 0xE3C, 0x18);
        }
    }
    SB(0x910) = 0x1D;
    func_088D3A04(self->sub + 4);
    func_088D3910(0x1D, self->sub + 4, D_08AA3BE0, 0, 1);
    a = SB(0x8AE0);
    if (a != 0) {
        func_08AB3908(a, 2);
    }
    func_08AA1420(self);
    func_08A9CF8C(self);
    func_08A9C55C(self, 9);
    SB(0x81C4) = 9;
    a = self->sub;
    if ((*(unsigned *)(a + 0x81B4) & 0x40000) != 0) {
        func_08AAB960(*(int *)(a + 0x8044));
        SBU(0x81B4) = SBU(0x81B4) & 0xFFFBFFFF;
        a = self->sub;
    }
    if ((*(unsigned *)(a + 0x81B4) & 0x40) != 0) {
        {
            G98F00 *g = &D_09B6EFB0;
            In98F00 *i = &g->in;
            i->f3e0 = i->f3e0 | 1;
        }
    }
    func_08A9F190(self);
    func_08A9BEA0(self);
}
