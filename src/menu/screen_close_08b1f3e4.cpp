/* Closes a screen: base teardown, frees the block at 0x138, then runs the block's callback (read first). */
typedef void (*Cb)(int);
struct Block { Cb callback; };
struct Screen { char d[0x134]; int pool; Block *block; };
struct G { int a; int b; int c; };
extern "C" {
extern void *D_089D7830;
extern int D_08B01CF8;
extern G D_09B6DA30;
extern void func_08B1ED38(Screen *);
extern void func_08AF4A94(int);
extern void func_08AF7CD0(int);
extern void func_088D0370(void *, int, int);
extern void func_0885C538(int, Block *, int);
}
extern "C" void func_08B1F3E4(Screen *self)
{
    func_08B1ED38(self);
    func_08AF4A94(D_08B01CF8);
    int kind = 0xC;
    func_088D0370(D_089D7830, kind, 1);
    Cb callback = self->block->callback;
    if (self->pool != 0) {
        func_0885C538(self->pool, self->block, 0);
        self->block = 0;
    }
    func_08AF7CD0(D_08B01CF8);
    callback(0);
    D_09B6DA30.c = 0;
}
