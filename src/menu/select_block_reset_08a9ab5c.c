/* Resets the select block's state (flags, counts, five slots) and, when the global mode is 2, fills the slots from the saved records. */
typedef struct B9AB5C {
    char p0[0x7D4C]; unsigned char f4c;
    char p1[0x7D64 - 0x7D4D]; int w64;
    int cnt[5];
    unsigned char fa[5]; unsigned char fb[5];
    char p2[2];
    int val[5];
    int w9c;
    char p3[0x7E2C - 0x7DA0]; unsigned char f2c; unsigned char f2d;
    char p4[2];
    char list[1];
} B9AB5C;
typedef struct S9AB5C { char pad[0x154]; B9AB5C *b; } S9AB5C;
extern int D_09B6EFB0[];
extern void func_0892A5EC(void *l);
extern void func_0892A6B4(void *l, void *g);
extern void func_0892A768(void *l, int i, int n, int c);
void func_08A9AB5C(S9AB5C *self)
{
    int i;
    int off;
    int *g;
    self->b->f4c = 0;
    self->b->w64 = 5;
    self->b->w9c = 0;
    self->b->f2c = 0;
    self->b->f2d = 0;
    for (i = 0, off = 0; i < 5; i++, off += 4) {
        self->b->cnt[i] = 0;
        self->b->fa[i] = 0;
        self->b->fb[i] = 0;
        self->b->val[i] = -1;
    }
    func_0892A5EC(self->b->list);
    if (D_09B6EFB0[0] == 2) {
        func_0892A6B4(self->b->list, (char *)(g = D_09B6EFB0) + 8);
        for (i = 0; i < 5; i++, g += 4) {
            int n = g[10];
            if (n > 0) {
                int c = g[13];
                void *l = self->b->list;
                int val = g[11];
                func_0892A768(l, i, n, c);
                self->b->cnt[i] = n;
                self->b->val[i] = val;
            }
        }
    }
}
