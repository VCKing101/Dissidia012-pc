/* Menu list refresh: marks both windows busy, then clears the marks once the first enabled row is found. */
typedef struct { char p[0x20]; unsigned short flags; } W;
typedef struct {
    char p0[0x564]; W *w564; W *w568;
    char p1[0xFB8 - 0x56C]; int fFB8;
    char p2[0x1224 - 0xFBC]; int f1224;
    char p3[0x1430 - 0x1228]; int cnt[1];
} B;
typedef struct { char p[0xDB30]; B *b; } S;
extern void func_088D9D24(W *);
extern int func_08ACC4FC(S *, int);
void func_08ACC63C(S *self) {
    int i, lim;
    W *w;
    w = self->b->w564;
    w->flags |= 0x4000;
    func_088D9D24(w);
    w = self->b->w568;
    w->flags |= 0x4000;
    func_088D9D24(w);
    for (i = 0; i < self->b->fFB8; i++) {
        if (func_08ACC4FC(self, i)) {
            w = self->b->w564;
            w->flags &= ~0x4000;
            func_088D9D24(w);
            break;
        }
    }
    if (self->b->f1224 < 4) {
        lim = self->b->cnt[self->b->f1224] + 1;
    } else {
        lim = self->b->cnt[self->b->f1224];
    }
    for (i = self->b->fFB8 + 8; i < lim; i++) {
        if (func_08ACC4FC(self, i)) {
            w = self->b->w568;
            w->flags &= ~0x4000;
            func_088D9D24(w);
            break;
        }
    }
}
