/* Runs func_08AF7084 then a chain of checks on D_08B01CF8; if all pass, sets word +0x1B78 of the block at self+0x46AC to 3. Always returns 0. */
typedef struct { char p0[0x1B78]; int state; } Blk;
typedef struct { char p0[0x46AC]; Blk *blk; } Obj;
extern int D_08B01CF8;
extern int D_089D69E8;
extern void func_08AF7084(int a);
extern int func_08AF70C4(int a);
extern int func_08AF7504(int a);
extern int func_08885608(int a, int b);
int func_08ADC6CC(Obj *self) {
    func_08AF7084(D_08B01CF8);
    if (func_08AF70C4(D_08B01CF8) == 0) {
        return 0;
    }
    if (func_08AF7504(D_08B01CF8) == 0) {
        return 0;
    }
    if (func_08885608(D_089D69E8, 1) == 0) {
        return 0;
    }
    self->blk->state = 3;
    return 0;
}
