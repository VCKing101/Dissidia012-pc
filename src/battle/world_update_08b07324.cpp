struct G10 { char p0[0x4C]; int i4c; char p1[4]; int i54; char p2[0x7C8 - 0x58]; int i7c8; };
struct G00 { char p0[4]; int i4; char p1[0x18 - 8]; unsigned char b18; };
struct G85 { char p0[2]; char b2; char p1[0x279C - 3]; unsigned u279c; };
struct Ch { char p[0x10CC]; short s10cc; };
struct Obj { char p0[0x20]; Ch *a20; Ch *a24; char p1[4]; float f2c; };
extern "C" {
extern G10 *D_089D6A10;
extern G00 *D_089D6A00;
extern G85 *D_089D8508;
extern unsigned char D_089D5F1F;
extern int *D_089D8364;
extern const float D_089C9F54;
extern int D_08B05258, D_08B1FE60, D_08B212E8;
int func_088F47E0(int);
int func_088CA528(int);
int func_08B1DBD0(int);
int func_08B21018(int);
void func_08B05040(int, int);
int func_0887CFE4(void);
void func_0887D2A8(int);
void func_08B0AD88(Obj *, int, int, int);
void func_08B0B71C(Obj *, int);
void func_089072AC(void *, int);
}
extern "C" void func_08B07324(Obj *self) {
    G10 *g = D_089D6A10;
    int mode = g->i4c;
    G00 *h = D_089D6A00;
    unsigned char bv = 0;
    if (mode == 0xC && D_089D8508->b2 == 3) {
        bv = (D_089D8508->u279c & 2) != 0;
    }
    if (D_089D5F1F == 0 && (unsigned char)(func_088F47E0(0x12) != 0) == 1 && mode != 9 && bv == 0) {
        int arg;
        int a7 = g->i7c8;
        int r;
        arg = D_08B05258;
        r = func_088CA528(a7);
        if (r == 1 && func_08B1DBD0(D_08B1FE60) == 0 && func_08B21018(D_08B212E8) == 0 && h->b18 == 0) {
            func_08B05040(arg, 0);
        } else {
            func_08B05040(arg, 1);
        }
    }
    {
        int a54 = D_089D6A10->i54;
        int v = 0;
        if (h->b18 == 1) v = h->i4;
        func_0887D2A8(func_0887CFE4());
        func_08B0AD88(self, self->a20->s10cc, v, 1);
        func_08B0AD88(self, self->a24->s10cc, v, 0);
        func_08B0B71C(self, a54);
    }
    if (D_089D6A10->i4c == 5) {
        int *q = D_089D8364;
        if (q != 0) func_089072AC(q, 1);
    }
    self->f2c = D_089C9F54 * 3.0f;
}
