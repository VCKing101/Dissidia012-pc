/* Steps the page's selection back one entry (or wraps to the last), refreshes, and flags the page; returns whether it moved. */
typedef struct Bg { char p0[0x778]; int h778; int h77C; char p1[0xCAC0 - 0x780]; unsigned int fCAC0; int cAC4; int cAC8; int pad; int cAD0; int cAD4; int cAD8; } Bg;
typedef struct S { char p[0xDB30]; Bg *b; } S;
extern void func_08AD030C(S *s);
extern void func_08AD07F0(S *s);
extern void func_088D9F5C(int h, int a, int b);
int func_08AD0460(S *self)
{
    int moved = 0;
    Bg *b = self->b;
    if (b->cAC4 != 0) {
        if (b->cAC8 == 1) {
            if (b->cAD4 != 0) {
                b->cAD4 = b->cAD4 - 1;
                func_08AD030C(self);
                func_088D9F5C(self->b->h77C, 7, 1);
                moved = 1;
            }
        } else {
            b->cAD4 = b->cAD8 - 1;
            func_08AD030C(self);
            func_088D9F5C(self->b->h778, 7, 1);
            moved = 1;
            self->b->cAC8 = 1;
        }
    }
    if (moved != 0) {
        func_08AD07F0(self);
        self->b->fCAC0 = self->b->fCAC0 | 2;
    }
    return moved;
}
