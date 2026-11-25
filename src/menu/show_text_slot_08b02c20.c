/* Frees the text handle in slot idx of the block at self+0x67C (+0x5D4), formats a0..a2 into a 128-byte buffer, shows it with func_0887D868 and refreshes the slot. */
typedef struct { char p0[0x5D4]; int h[1]; } Blk;
typedef struct { char p0[0x67C]; Blk *blk; } Obj;
typedef struct { short a; short b; int c; int d; } Rec;
extern int D_089D6998;
extern Rec *func_08B04610(void);
extern void func_0887BA5C(void);
extern void func_0887DF14(int a, void *dst, int n, int src);
extern int func_0887D868(int a, int b, void *c, int d, int e, int f, int g, int h);
extern void func_0887B924(int h);
extern void func_08B02A40(Obj *o, int i, int v);
extern void func_08B03584(Obj *o, int a, int i);
void func_08B02C20(Obj *self, int idx, int a2) {
    int pad0[1];
    int sv[1];
    char buf[128];
    char *bp;
    int off;
    Blk *b;
    sv[0] = ((short *)((char *)func_08B04610() + idx * 12))[1];
    b = self->blk;
    off = idx * 4;
    if (*(int *)((char *)b + off + 0x5D4) != 0) {
        func_0887BA5C();
        *(int *)((char *)self->blk + off + 0x5D4) = 0;
    }
    int gp = D_089D6998;
    bp = buf;
    func_0887DF14(gp, bp, 0x80, a2);
    *(int *)((char *)self->blk + off + 0x5D4) = func_0887D868(D_089D6998, 0, bp, -1, 0, 0, 0, 0);
    func_0887B924(*(int *)((char *)self->blk + off + 0x5D4));
    func_08B02A40(self, idx, sv[0]);
    func_08B03584(self, 0, idx);
}
