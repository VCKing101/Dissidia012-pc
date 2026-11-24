/* Runs func_08AF7084 and a chain of checks on D_08B01CF8; if all pass, sets word +0x638 of the block at self+0x67C to 3. Always returns 0. */
typedef struct { char p0[0x638]; int state; } Blk;
typedef struct { char p0[0x67C]; Blk *blk; } Obj;
typedef struct { char p0[0x2D20]; unsigned char flag; } Inner;
typedef struct { char p0[0x140]; Inner *inner; } Root;
extern Root *D_08B01CF8;
extern int D_089D69E8;
extern void func_08AF7084(Root *a);
extern int func_08AF70C4(Root *a);
extern int func_08885608(int a, int b);
int func_08B020AC(Obj *self) {
    func_08AF7084(D_08B01CF8);
    if (func_08AF70C4(D_08B01CF8) == 0) {
        return 0;
    }
    if (func_08885608(D_089D69E8, 1) == 0) {
        return 0;
    }
    if (D_08B01CF8->inner->flag == 0) {
        return 0;
    }
    self->blk->state = 3;
    return 0;
}
