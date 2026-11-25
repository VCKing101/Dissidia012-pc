/* Starts the page's message box: fills its request block (text, buttons, callbacks) and opens it once; returns 1 when started. */
typedef struct Bk {
    char p0[0x9E4]; char w9E4[0xA00 - 0x9E4]; int cA00; char p1[0xA14 - 0xA04]; int cA14; int pA18; char p2[0xA24 - 0xA1C];
    int cA24; char p3[0xA2C - 0xA28]; int cA2C; char p4[0xA34 - 0xA30]; int cA34; int cA38; char p5[0xA40 - 0xA3C];
    int cA40; int pA44; int cA48; int pA4C; char p6[0xB10 - 0xA50]; char t10[0xB2A - 0xB10]; char t2A[0x1A]; char p7[0xB59 - 0xB44]; char cB59;
} Bk;
typedef struct S { char p[0x14C]; Bk *b; } S;
extern unsigned char *D_089D7280;
extern void func_088FE9E8(void);
extern void func_08935828(void *d, int v, int n);
extern void func_08935524(void *d, void *s, int n);
extern int func_08ABBC80(S *s, int k);
extern int func_089C58B8(void *w);
extern void func_088B16FC(void *p);
extern void func_088729C0(void);
int func_08ABC248(S *self)
{
    int one;
    if (*D_089D7280 != 0) {
        return 0;
    } else {
        func_088FE9E8();
        self->b->cB59 = 0;
        self->b->cA14 = one = 1;
        self->b->pA18 = (int)((char *)self->b + 0xA24);
        self->b->cA00 = 0;
        func_08935828((char *)self->b + 0xA24, 0, 0x34);
        func_08935524((char *)self->b + 0xB10, (char *)self->b + 0xB2A, 0x1A);
        self->b->cA24 = one;
        self->b->cA2C = 0;
        self->b->cA34 = 0;
        self->b->cA40 = func_08ABBC80(self, 3);
        self->b->pA44 = (int)((char *)self->b + 0xB10);
        self->b->cA48 = 0xD;
        self->b->pA4C = (int)((char *)self->b + 0xB10);
        self->b->cA38 = 1;
        if (func_089C58B8((char *)self->b + 0x9E4) != 0) {
            return 0;
        } else {
            func_088B16FC(D_089D7280);
            *D_089D7280 = one;
            func_088729C0();
            return one;
        }
    }
}
