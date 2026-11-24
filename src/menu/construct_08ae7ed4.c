/* Constructor: sets the vtable, points self+0xA690 at the embedded block (self+4), clears its fields and a 13-entry array, then runs the two init calls. */
typedef struct { char p0[0x2554]; int w2554; char p1[4]; int w255C; int w2560; char p2[0x2604 - 0x2564]; int w2604; char p3[0x2694 - 0x2608]; int w2694; int w2698; char p4[0x9504 - 0x269C]; int arr[13]; } Blk;
typedef struct { void *vt; char p0[0xA690 - 4]; Blk *blk; } Obj;
typedef struct { char p0[0xDC]; int mode; } Glob;
extern Glob D_09B6CDA0;
extern char D_08AEB118[];
extern void func_08AF1230(Obj *self);
extern void func_08AE7760(Obj *self);
extern void func_08AEAB28(Obj *self);
Obj *func_08AE7ED4(Obj *self) {
    Blk *b;
    int i;
    func_08AF1230(self);
    self->vt = D_08AEB118;
    b = (Blk *)((char *)self + 4);
    self->blk = b;
    b->w2554 = 0;
    self->blk->w255C = 0;
    self->blk->w2560 = 0;
    self->blk->w2694 = D_09B6CDA0.mode;
    self->blk->w2698 = 0;
    for (i = 0; i < 13; i++) {
        self->blk->arr[i] = 0;
    }
    func_08AE7760(self);
    self->blk->w2604 = 0;
    func_08AEAB28(self);
    return self;
}
