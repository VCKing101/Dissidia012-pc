/* Initialises the owner's three list controls and loads the screen resources, then calls func_08AF7104. */
typedef struct { char p0[0xE]; short h; } Inner;
typedef struct {
    char p0[0x1C]; char b1C[1];
    char p1[0x41C - 0x1D]; char b41C[1];
    char p2[0x620 - 0x41D]; Inner *w620;
    char p3[0x89C - 0x624]; int w89C; int w8A0; char p4[4]; char b8A8[1];
    char p5[0x2E0C - 0x8A9]; char b2E0C[0x2E38 - 0x2E0C]; char b2E38[0x2E64 - 0x2E38]; char b2E64[1];
} Blk;
typedef struct { char p0[0x140]; Blk *blk; } Obj;
typedef struct { char p0[0xDC]; int mode; } Glob;
extern Glob D_09B6CDA0;
extern char D_08B00FFE[], D_08B010A0[], D_08B00F22[], D_08B00F66[], D_08B010AC[];
extern void func_088D46E4(void *a, int b, int c, int d, int e, int f);
extern void func_088D3F7C(int a, void *b, void *c, int d);
extern void func_088D3B88(void *a, void *b, void *c, int d, int e);
extern void func_088D3CF0(void *a, void *b, void *c);
extern void func_088D428C(void *a, void *b, void *c, int d);
extern void func_08AF7104(Obj *o);
void func_08AFA9FC(Obj *self) {
    func_088D46E4(self->blk->b2E0C, D_09B6CDA0.mode, 0, 5, 0, -1);
    func_088D46E4(self->blk->b2E38, 0, 0, 4, 0, -1);
    func_088D46E4(self->blk->b2E64, 0, 0, 4, 0, -1);
    func_088D3F7C(self->blk->w8A0, self->blk->b8A8, D_08B00FFE, 3);
    func_088D3F7C(self->blk->w89C, self->blk->b8A8, D_08B010A0, 0);
    func_088D3B88(self->blk->b1C, self->blk->b41C, D_08B00F22, 0, 0x120);
    func_088D3CF0(self->blk->b1C, self->blk->b41C, D_08B00F66);
    self->blk->w620->h = 6;
    func_088D428C(self->blk->b41C, self->blk->b8A8, D_08B010AC, 1);
    func_08AF7104(self);
}
