/* Runs the setup calls for the screen: func_08AF4008, func_08AFB554, two func_088D428C list fills, func_08AF371C, func_08AF1BDC. */
typedef struct { char p0[0x1C]; char b1C[0x41C - 0x1C]; char b41C[0x8A8 - 0x41C]; char b8A8[1]; } Blk;
typedef struct { char p0[0x140]; Blk *blk; } Obj;
typedef struct { char p0[0xDC]; int mode; } Glob;
extern Glob D_09B6CDA0;
extern void func_08AF4008(Obj *o, int a);
extern void func_08AFB554(Obj *o);
extern void func_088D428C(void *a, void *b, void *c, int d);
extern void func_08AF371C(Obj *o, void *a, void *b);
extern void func_08AF1BDC(Obj *o);
extern char D_08B0057E[];
extern char D_08B005D6[];
void func_08AFB184(Obj *self) {
    Obj *me = self;
    func_08AF4008(me, D_09B6CDA0.mode);
    func_08AFB554(me);
    func_088D428C(me->blk->b41C, me->blk->b8A8, D_08B0057E, 1);
    func_088D428C(me->blk->b41C, me->blk->b8A8, D_08B005D6, 1);
    func_08AF371C(me, me->blk->b1C, me->blk->b41C);
    func_08AF1BDC(me);
}
