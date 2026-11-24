// Clears the fields and three blocks (+0 size 8, +0x14 size 0x8C, +0xA8 size 0x1A4); returns self.
extern void func_08935828(void *, int, int);
typedef struct { char pad0[8]; int w8, wc, w10; char blk14[0xA0 - 0x14]; int wa0, wa4; char blk_a8[0x24C - 0xA8]; unsigned char b24c, b24d; } Obj08B4B208;
Obj08B4B208 *func_08B4B208(Obj08B4B208 *self)
{
    self->w8 = 0;
    self->wc = 0;
    self->w10 = 0;
    int neg = -1;
    int one = 1;
    self->wa0 = 0;
    self->wa4 = neg;
    self->b24c = one;
    self->b24d = 0;
    func_08935828(self, 0, 8);
    func_08935828(self->blk14, 0, 0x8C);
    func_08935828(self->blk_a8, 0, 0x1A4);
    return self;
}
