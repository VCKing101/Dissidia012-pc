/* Constructor: sets the vtable, points self+0x23F0 at the embedded block (self+4), clears its fields, copies the mode word, then runs the two init calls. */
typedef struct { char p0[0x1920]; int w1920; char p1[4]; int w1928; int w192C; char p2[0x19B8 - 0x1930]; int w19B8; } Blk;
typedef struct { void *vt; char p0[0x23F0 - 4]; Blk *blk; } Obj;
typedef struct { char p0[0xDC]; int mode; } Glob;
extern Glob D_09B6CDA0;
extern char D_08AEF718[];
extern void func_08AF1230(Obj *self);
extern void func_08AED050(Obj *self);
extern void func_08AEE38C(Obj *self);
Obj *func_08AED360(Obj *self) {
    Blk *b;
    func_08AF1230(self);
    self->vt = D_08AEF718;
    b = (Blk *)((char *)self + 4);
    self->blk = b;
    b->w1920 = 0;
    self->blk->w1928 = 0;
    self->blk->w192C = 0;
    self->blk->w19B8 = D_09B6CDA0.mode;
    func_08AED050(self);
    func_08AEE38C(self);
    return self;
}
