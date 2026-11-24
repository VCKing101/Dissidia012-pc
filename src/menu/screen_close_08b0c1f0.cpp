/* Closes a screen: base teardown, frees the block at 0x138 after its cleanup, then runs the block's callback. */
typedef void (*Cb)(int);
struct Block { Cb callback; };
struct Screen { char d[0x134]; int pool; Block *block; };
struct G { char d[0x168]; int c; };
extern "C" {
extern void *D_089D7830;
extern int D_08B01CF8;
extern G D_09B6D868;
extern void func_08B0B5D0(Screen *);
extern void func_08B0C468(Screen *);
extern void func_08AF7CD0(int);
extern void func_088D0370(void *, int, int);
extern void func_0885C538(int, Block *, int);
}
extern "C" void func_08B0C1F0(Screen *self)
{
    func_08B0B5D0(self);
    int kind = 8;
    func_088D0370(D_089D7830, kind, 1);
    Cb callback = self->block->callback;
    if (self->pool != 0) {
        func_08B0C468(self);
        func_0885C538(self->pool, self->block, 0);
        self->block = 0;
    }
    func_08AF7CD0(D_08B01CF8);
    callback(0);
    D_09B6D868.c = 0;
}
