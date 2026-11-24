/* Constructor: sets the vtable, points self+0x46AC at the embedded block (self+4), clears its fields and a 13-entry array, then runs the two init calls. */
typedef struct { char p0[0x1B78]; int w1B78; char p1[4]; int w1B80; int w1B84; char p2[0x1C34 - 0x1B88]; int w1C34; char p3[0x1CA4 - 0x1C38]; int w1CA4; int w1CA8; char p4[0x35F4 - 0x1CAC]; int arr[13]; } Blk;
typedef struct { void *vt; char p0[0x46AC - 4]; Blk *blk; } Obj;
typedef struct { char p0[0xDC]; int mode; } Glob;
extern Glob D_09B6CDA0;
extern char D_08ADF498[];
extern void func_08AF1230(Obj *self);
extern void func_08ADC7F4(Obj *self);
extern void func_08ADDC44(Obj *self);
Obj *func_08ADCB50(Obj *self) {
    Blk *b;
    int i;
    func_08AF1230(self);
    self->vt = D_08ADF498;
    b = (Blk *)((char *)self + 4);
    self->blk = b;
    b->w1B78 = 0;
    self->blk->w1B80 = 0;
    self->blk->w1B84 = 0;
    self->blk->w1CA4 = D_09B6CDA0.mode;
    self->blk->w1CA8 = 10;
    for (i = 0; i < 13; i++) {
        self->blk->arr[i] = 0;
    }
    func_08ADC7F4(self);
    self->blk->w1C34 = 0;
    func_08ADDC44(self);
    return self;
}
