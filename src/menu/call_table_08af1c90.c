/* Calls the function at D_08B0155C[self->p140->i], stores its result at +0xC04, sets +0xB3C to 9, returns 1. */
typedef struct { char p0[0xB3C]; int s; char p1[0xB48 - 0xB40]; int i; char p2[0xC04 - 0xB4C]; int res; } Blk;
typedef struct { char p0[0x140]; Blk *blk; } Obj;
extern int (*D_08B0155C[])(void);
int func_08AF1C90(Obj *self) {
    self->blk->res = D_08B0155C[self->blk->i]();
    self->blk->s = 9;
    return 1;
}
