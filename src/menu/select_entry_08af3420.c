/* Resets via func_08AF2AD0, stores the chosen index in D_09B6CDA0+8, copies its table word into the block, then updates the display. */
typedef struct { char p0[8]; int sel; int tbl[1]; } Glob;
typedef struct { int first; } Blk0;
typedef struct { char p0[0x140]; Blk0 *blk; } Obj;
extern Glob D_09B6CDA0;
extern void func_08AF2AD0(void);
extern void func_08AF2948(Obj *o);
extern void func_088D46E4(void *a, int b, int c, int d, int e, int f);
void func_08AF3420(Obj *self, int idx) {
    Obj *me = self;
    int i = idx;
    func_08AF2AD0();
    D_09B6CDA0.sel = i;
    me->blk->first = D_09B6CDA0.tbl[i];
    func_08AF2948(me);
    func_088D46E4((char *)me->blk + 0xB84, *(int *)((char *)&D_09B6CDA0 + 0xD8), 0, 0xD, 0, -1);
}
