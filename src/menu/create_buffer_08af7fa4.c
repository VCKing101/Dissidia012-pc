/* Calls func_0885C6CC with self->p138, 4 * D_089D76B0->w8, 0, 0, 1, 0 and stores the result in self->p140+0xB64. */
typedef struct { char p0[8]; int w8; } Cfg;
typedef struct { char p0[0xB64]; int res; } Blk;
typedef struct { char p0[0x138]; int p138; int pad; Blk *blk; } Obj;
extern Cfg *D_089D76B0;
extern int func_0885C6CC(int a, int b, int c, int d, int e, int f);
void func_08AF7FA4(Obj *self) {
    int a = self->p138;
    int n = D_089D76B0->w8 << 2;
    int one = 1;
    self->blk->res = func_0885C6CC(a, n, 0, 0, one, 0);
}
