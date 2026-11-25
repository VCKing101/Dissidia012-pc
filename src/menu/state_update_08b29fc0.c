/* Menu state update: runs a lookup, then either the full refresh (result 1) or the short one, and sets state 6. */
typedef struct {
    char p0[4]; int f4; char p1[0x18 - 8]; int f18; int f1C;
    char p2[0x49C - 0x20]; char a49C[1];
    char p3[0x740 - 0x49D]; int f740;
} B;
typedef struct { char p[0xA18]; B *b; } S;
typedef struct { char p[0x27]; unsigned char b27; char q[0xF0 - 0x28]; short h0F0; } G;
extern char *D_089D712C;
extern void func_088D9F5C(int, int, int);
extern void func_0892A5EC(char *);
extern void func_08B2A82C(S *);
extern void func_08B2A970(S *);
extern void func_08B2AA04(S *);
extern void func_08B2ABF8(S *);
extern void func_08B2AE90(S *);
extern void func_08B2AFF0(S *);
extern void func_08B2B0C4(S *);
extern void func_08B2B51C(S *, int, void *, void *);
extern void func_08B2C330(S *);
extern int func_08B2C908(S *, int, void *, void *);
int func_08B29FC0(S *self) {
    int r, go;
    B *b;
    func_08B2A970(self);
    b = self->b;
    r = func_08B2C908(self, b->f18, &b->f1C, b->a49C);
    if (r != 0) {
        b = self->b;
        func_08B2B51C(self, b->f18, &b->f1C, b->a49C);
        if (r == 1) {
            G *g;
            func_0892A5EC(D_089D712C + 0x3E758 + self->b->f740 * 0x48 + 4);
            g = (G *)(D_089D712C + 0x1FDD8);
            go = (g->b27 & 0x40) != 0;
            go = (unsigned char)go;
            if (!go) {
                if (g->h0F0 == self->b->f740) {
                    g->b27 |= 0x40;
                    g->h0F0 = -1;
                }
            }
            func_08B2A82C(self);
            func_08B2AA04(self);
            func_08B2ABF8(self);
            func_08B2C330(self);
            func_08B2AE90(self);
            func_088D9F5C(self->b->f1C, 9, 1);
            func_08B2AFF0(self);
            b = self->b;
        } else {
            func_08B2B0C4(self);
            b = self->b;
        }
        b->f4 = 6;
    }
    return 0;
}
