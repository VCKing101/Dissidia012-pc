// Stores a1 at +0xA0, then calls func_08B4B504 when the byte at +0x24C is 1, else func_08B4B570.
extern void func_08B4B504(void *);
extern void func_08B4B570(void *);
typedef struct { char pad0[0xA0]; int v; char pad1[0x24C - 0xA4]; unsigned char mode; } Obj08B4B834;
void func_08B4B834(Obj08B4B834 *self, int v)
{
    int one = 1;
    self->v = v;
    if (self->mode == one) {
        func_08B4B504(self);
    } else {
        func_08B4B570(self);
    }
}
