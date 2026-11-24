/* If the halfword at self->p140+0x2E04 differs from v, calls func_0888550C(D_089D69E8, +0x89C word, 1, v) and stores v there. */
typedef struct { char p0[0x89C]; int w89C; char p1[0x2E04 - 0x8A0]; unsigned short cur; } Blk;
typedef struct { char p0[0x140]; Blk *blk; } Obj;
extern int D_089D69E8;
extern void func_0888550C(int a, int b, int c, int d);
void func_08AF7134(Obj *self, unsigned short v) {
    unsigned int val = v;
    if (self->blk->cur != val) {
        func_0888550C(D_089D69E8, self->blk->w89C, 1, val);
        self->blk->cur = val;
    }
}
