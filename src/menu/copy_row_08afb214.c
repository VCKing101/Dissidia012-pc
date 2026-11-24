/* Copies the 0xC6-byte row a1 over row a2 of the table at owner+0xC80, then refreshes the screen with the update calls. */
typedef struct { short h[99]; } Row;
typedef struct { char p0[0x1C]; char b1C[1]; char p1[0x41C - 0x1D]; char b41C[1]; char p2[0x8A8 - 0x41D]; char b8A8[1]; char p3[0xC80 - 0x8A9]; Row rows[1]; } Blk;
typedef struct { char p0[0x140]; Blk *blk; } Obj;
extern char D_08B0057E[], D_08B005D6[];
extern void func_08AF3ECC(Obj *o);
extern void func_08AFD194(Obj *o, int a, int b);
extern void func_08AFB554(Obj *o);
extern void func_088D428C(void *a, void *b, void *c, int d);
extern void func_08AF371C(Obj *o, void *a, void *b);
extern void func_08AF1BDC(Obj *o);
void func_08AFB214(Obj *self, int from, int to) {
    Row *src = &self->blk->rows[from];
    Row *dst = &self->blk->rows[to];
    *dst = *src;
    func_08AF3ECC(self);
    func_08AFD194(self, 1, 0);
    func_08AFB554(self);
    func_088D428C(self->blk->b41C, self->blk->b8A8, D_08B0057E, 1);
    func_088D428C(self->blk->b41C, self->blk->b8A8, D_08B005D6, 1);
    func_08AF371C(self, self->blk->b1C, self->blk->b41C);
    func_08AF1BDC(self);
}
