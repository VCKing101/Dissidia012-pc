/* Stores a mode word in D_09B6CDA0+0xDC and byte +0x13E3 of the owner's block, then runs func_08AF3ECC and func_08AFD194(self,1,0). */
typedef struct { char p0[0xDC]; int mode; } Glob;
typedef struct { char p0[0x13E3]; char m; } Blk;
typedef struct { char p0[0x140]; Blk *blk; } Obj;
extern Glob D_09B6CDA0;
extern void func_08AF3ECC(Obj *o);
extern void func_08AFD194(Obj *o, int a, int b);
void func_08AF4008(Obj *self, int mode) {
    D_09B6CDA0.mode = mode;
    self->blk->m = mode;
    func_08AF3ECC(self);
    func_08AFD194(self, 1, 0);
}
