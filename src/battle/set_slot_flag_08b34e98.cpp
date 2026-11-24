// For slot index < 6: marks byte 0x1268+slot and sets bit 29 of the word at +0x380.
struct Obj08B34E98 {
    char pad0[0x380];
    unsigned flags;
    char pad1[0x1268 - 0x384];
    unsigned char slot[6];
};
extern "C" void func_08B34E98(Obj08B34E98 *self, unsigned slot)
{
    if (slot >= 6) {
        return;
    }
    self->slot[slot] = 1;
    self->flags |= 0x20000000;
}
