/* Tears down and recreates the screen's buffers: func_0887E684, free +0x8D8, func_088D0B2C, allocate 0xBA800 into +0xB88, register it with func_08AB80E4 and func_08865184. */
typedef struct { char p0[0x8D8]; int w8D8; char p1[0xB88 - 0x8DC]; int wB88; } Blk;
typedef struct { char p0[0x100]; char b100[0x134 - 0x100]; int p134; char p1[0x14C - 0x138]; Blk *blk; } Obj;
extern int D_089D6998;
extern int D_089D7830;
extern int D_08ABB9E4;
extern void func_0887E684(int a);
extern void func_0885C538(int a, int b, int c);
extern void func_088D0B2C(int a, int b);
extern int func_0885C6CC(int a, int b, int c, int d, int e, int f);
extern void func_08AB8080(int a);
extern void func_08AB80E4(int a, int b, void *cb, int c, int d, int e, int f);
extern void func_08ABF468(void);
extern void func_08865184(void *p);
void func_08ABF48C(Obj *self) {
    int one;
    func_0887E684(D_089D6998);
    func_0885C538(self->p134, self->blk->w8D8, 0);
    self->blk->w8D8 = 0;
    func_088D0B2C(D_089D7830, 10);
    one = 1;
    self->blk->wB88 = func_0885C6CC(self->p134, 0xBA800, 0, 0, one, 0);
    func_08AB8080(D_08ABB9E4);
    func_08AB80E4(D_08ABB9E4, self->blk->wB88, func_08ABF468, 0, 1, 0, 0);
    func_08865184(self->b100);
}
