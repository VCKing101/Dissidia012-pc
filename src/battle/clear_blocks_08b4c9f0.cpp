// Clears two words and three blocks (+8 size 0x20, +0x28 size 0xA8, +0xD0 size 0x78), sets six words (stride 0x14 from +0xE0) to -1; returns self.
extern "C" void func_08935828(void *, int, int);
struct Slot08B4C9F0 { int v; char pad[0x10]; };
struct Obj08B4C9F0 { int w0, w4; char blk8[0x20]; char blk28[0xA8]; char blkd0[0x10]; Slot08B4C9F0 slot[6]; };
extern "C" Obj08B4C9F0 *func_08B4C9F0(Obj08B4C9F0 *self)
{
    self->w0 = 0;
    self->w4 = 0;
    func_08935828(self->blk8, 0, 0x20);
    func_08935828(self->blk28, 0, 0xA8);
    func_08935828(self->blkd0, 0, 0x78);
    for (int i = 0, neg = -1; i < 6; i++) {
        self->slot[i].v = neg;
    }
    return self;
}
