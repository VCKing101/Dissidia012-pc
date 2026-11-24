/* Teardown: stops the task if running, frees the block at 0xCC and clears the pointer. */
typedef struct { int d[2]; int p; int q; unsigned char flag; } Blk;
typedef struct { char d[0xCC]; Blk *blk; } Self;
extern int D_089D6C30;
extern void func_0889F340(int, int);
extern void func_0889F2B8(int);
extern void func_08B42BEC(void);
extern int func_08B42B40(void);
extern void func_0885C538(int, Blk *, int);
void func_08B3C248(Self *self)
{
    Blk *b = self->blk;
    if (b->flag != 0) {
        func_0889F340(D_089D6C30, 0);
        func_0889F2B8(D_089D6C30);
        b = self->blk;
    }
    if (b->p != 0) {
        func_08B42BEC();
        self->blk->p = 0;
    }
    func_0885C538(func_08B42B40(), self->blk, 0);
    self->blk = 0;
}
