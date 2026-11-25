typedef struct Wk {
    unsigned int f0; int f4; char pad8[0x12 - 8]; unsigned short f12; char pad14[0x18 - 0x14]; int f18;
    char f1C[0xC]; int f28;
    char pad2C[0x49C - 0x2C]; int f49C;
    char pad4A0[0x534 - 0x4A0]; int f534; int f538;
    char pad53C[0x629 - 0x53C]; unsigned char f629;
    char pad62A[0x674 - 0x62A]; char f674[0x30]; char f6A4[0x18]; int f6BC; char pad6C0[4]; int f6C4;
    char pad6C8[0x824 - 0x6C8]; int f824; int f828;
    char pad82C[0x834 - 0x82C]; int f834;
} Wk;
typedef struct Sc { char pad[0x844]; Wk *wk; } Sc;
extern char D_08B2F510[], D_08B2F564[], D_08B2F60C[];
extern void func_088D3B88(int, char *, char *, int, int);
extern void func_088D3CF0(int, char *, char *);
extern void func_0887BA5C(int);
extern int func_0887B3EC(int, int, int, int, int, int);
extern void func_088D428C(char *, int *, char *, unsigned char);
extern void func_088D9F5C(int, int, unsigned char);
extern void func_088D46E4(char *, int, int, int, int, int);
extern void func_08B2D9FC(Sc *);
extern void func_08B2D994(Sc *);
extern void func_088D4788(char *, char *, int);
extern void func_088D4928(char *, int);
extern void func_08B2DCEC(Sc *);
extern void func_08B2DD60(Sc *);
extern void func_08B2DEC4(Sc *);
extern void func_08B2DA9C(Sc *);
extern void func_08B2F37C(Sc *);
extern void func_08A80F14(int, unsigned char);
extern void func_08B2DE2C(Sc *);
extern void func_08B2EE88(Sc *);
extern void func_08B2E164(Sc *, int);
extern void func_08B2E9C8(Sc *);
int func_08B2E7D4(Sc *self)
{
    Wk *w;
    int v;
    w = self->wk;
    func_088D3B88(w->f18, w->f1C, D_08B2F510, w->f12, 0x120);
    func_088D3CF0(self->wk->f18, self->wk->f1C, D_08B2F564);
    w = self->wk;
    if (w->f49C != 0) {
        func_0887BA5C(w->f49C);
        self->wk->f49C = 0;
        w = self->wk;
    }
    self->wk->f49C = func_0887B3EC(w->f534, w->f538, 0, 10, 0, 0);
    func_088D428C(self->wk->f1C, &self->wk->f49C, D_08B2F60C, 1);
    w = self->wk;
    if (w->f834 == 1) {
        func_088D9F5C(w->f28, 2, 1);
        w = self->wk;
    }
    func_088D46E4(w->f674, w->f828, 0, 4, 0, -1);
    func_08B2D9FC(self);
    func_08B2D994(self);
    w = self->wk;
    func_088D4788(w->f674, w->f6A4, w->f6BC);
    w = self->wk;
    if (w->f629 != 0 && (w->f0 & 0x10) == 0) {
        w->f0 = w->f0 & 0xFFFFFFEF;
        func_088D4928(self->wk->f674, 5);
    }
    func_08B2DCEC(self);
    func_08B2DD60(self);
    func_08B2DEC4(self);
    func_08B2DA9C(self);
    func_08B2F37C(self);
    w = self->wk;
    if (w->f824 == 4) {
        func_08A80F14(w->f6C4, 1);
        func_08B2DE2C(self);
        self->wk->f4 = 4;
        func_08B2EE88(self);
    } else {
        func_08A80F14(w->f6C4, 0);
        func_08B2E164(self, 0);
        self->wk->f4 = 2;
        func_08B2E9C8(self);
    }
    return 0;
}
