/* Builds the screen layout: loads the two resources, writes four rectangles (shifted by the +0xE8 offset) into the block at +0xBC0 and registers them. */
typedef struct { short x, y, w, h; } Rc;
typedef struct {
    int a;
    int res;
    char p0[0xC - 8]; char b0C[1];
    char p1[0xE8 - 0xD];
    int off;
    char p2[0xBC0 - 0xEC];
    short r[8];
    char b0BD0[1];
} Blk;
typedef struct { char p0[0xC18]; Blk *blk; } Obj;
extern char D_08ACA39E[];
extern char D_08ACA3FE[];
extern void func_088D3CF0(int a, int b, void *c);
extern void func_088D9F5C(int a, int b, int c);
extern void func_088D4664(int a, void *b, void *c);
extern void func_088D428C(int a, void *b, void *c, int d);
extern void func_08AC7700(Obj *o, void *p);
void func_08AC8068(Obj *self) {
    int res = self->blk->res;
    char *b0c;
    int one;
    int off;
    func_088D3CF0(self->blk->a, res, D_08ACA39E);
    one = 1;
    func_088D9F5C(*(int *)((char *)res + self->blk->off * 4 + 0x13C), 8, 1);
    b0c = self->blk->b0C;
    func_088D4664(res, b0c, self->blk->r);
    func_088D428C(res, b0c, D_08ACA3FE, 1);
    off = self->blk->off;
    self->blk->r[0] = off + 0x10;
    self->blk->r[1] = 0x12;
    self->blk->r[2] = off + 0x4F;
    self->blk->r[3] = 0;
    self->blk->r[4] = off + 0xD;
    self->blk->r[5] = 0x12;
    self->blk->r[6] = off + 0x4F;
    self->blk->r[7] = one;
    func_08AC7700(self, self->blk->b0BD0);
    func_088D428C(res, b0c, self->blk->r, 1);
}
