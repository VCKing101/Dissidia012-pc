/* Calls func_08ACAFDC(self); stores -1 (or the returned rect's halfwords) into the block at self+0x1408.. and sets +0x1414 to 2. */
typedef struct { short x, y, w, h; } Rect;
typedef struct { char p0[0x1408]; int a; Rect r; int b; } Blk;
typedef struct { char p0[0xDB30]; Blk *blk; } Obj;
extern Rect *func_08ACAFDC(void *self);
void func_08ACB1FC(Obj *self) {
    Obj *me = self;
    Rect *r = func_08ACAFDC(me);
    if (r == 0) {
        me->blk->a = -1;
    } else {
        me->blk->a = r->x;
        me->blk->r = *r;
    }
    me->blk->b = 2;
}
