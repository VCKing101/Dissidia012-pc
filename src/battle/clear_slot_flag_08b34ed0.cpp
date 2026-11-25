// For slot index < 6: clears byte 0x1268+slot; if no slot byte is still 1, clears bit 29 of the word at +0x380.
struct Obj08B34ED0 { char pad0[0x380]; unsigned flags; char pad1[0x1268 - 0x384]; unsigned char slot[6]; };
extern "C" void func_08B34ED0(Obj08B34ED0 *self, unsigned idx)
{
    if (idx >= 6) {
        return;
    }
    self->slot[idx] = 0;
    for (int i = 0, one = 1; i < 6; i++) { if (*((unsigned char *)self + 0x1268 + i) == one) return; }
    self->flags &= 0xDFFFFFFF;
}
