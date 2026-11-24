/* Copies the saved selection into the owner's block, sets its flag byte, and creates the view object from D_09B6CDA0+0x9CC with the original or the function pair, storing the result at +0xA58. */
typedef struct { int sel; int tbl[1]; } Dummy;
typedef struct { char p0[8]; int sel; int tbl[(0xC8 - 0xC) / 4]; int w0C8; int w0CC; char p1[0x9CC - 0xD0]; char b9CC[0xA58 - 0x9CC]; int wA58; } Glob;
typedef struct { int w0; int w4; int w8; char p0[0x2D20 - 12]; unsigned char b2D20; } Blk;
typedef struct { char p0[0x140]; Blk *blk; } Obj;
typedef struct { char p0[0x1C]; unsigned int flags; } Host;
extern Glob D_09B6CDA0;
extern Host *D_089D8140;
extern int func_08AF9F68(Obj *o);
extern int func_08AF4240(Obj *o, void *a, int b, int c);
extern int func_08900C90(Host *h, void *a, int b);
void func_08AF7D94(Obj *self) {
    self->blk->w0 = D_09B6CDA0.tbl[D_09B6CDA0.sel];
    self->blk->w4 = D_09B6CDA0.w0C8;
    self->blk->w8 = D_09B6CDA0.w0CC;
    self->blk->b2D20 = 1;
    D_09B6CDA0.wA58 = 0;
    if ((D_089D8140->flags & 1) != 0) {
        D_09B6CDA0.wA58 = func_08900C90(D_089D8140, D_09B6CDA0.b9CC, 0x23);
    } else {
        D_09B6CDA0.wA58 = func_08AF4240(self, D_09B6CDA0.b9CC, 0x23, func_08AF9F68(self));
    }
}
